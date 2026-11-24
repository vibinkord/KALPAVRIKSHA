#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"

typedef enum { BATSMAN = 1, BOWLER = 2, ALLROUNDER = 3 } Role;

typedef struct PlayerNode {
    int id;
    char name[51];
    char team[51];
    Role role;
    int runs;
    float avg, sr;
    int wickets;
    float er;
    float pi;
    struct PlayerNode *next;
} PlayerNode;

typedef struct Team {
    int id;
    char name[51];
    int total;
    float srSum;
    int srCount;
    PlayerNode *head[4];
    struct Team *next;
} Team;

Team *teamHead = NULL;
Team *teamIdx[10];

/* ---------- input helpers ---------- */
int readInt(int *out) {
    char buf[100];
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    return sscanf(buf, "%d", out) == 1;
}

int readFloat(float *out) {
    char buf[100];
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    return sscanf(buf, "%f", out) == 1;
}

/* ---------- core ---------- */
float calcPi(PlayerNode *p) {
    if (p->role == BATSMAN) {
        return (p->avg * p->sr) / 100;
    } else if (p->role == BOWLER) {
        return (p->wickets * 2) + (100 - p->er);
    } else if (p->role == ALLROUNDER) {
        return ((p->avg * p->sr) / 100) + (p->wickets * 2);
    }
    return 0.0;
}

Team *createTeam(int id, const char *name) {
    Team *team = (Team *)calloc(1, sizeof(Team));
    team->id = id;
    strncpy(team->name, name, 50);
    if (!teamHead) teamHead = team;
    else {
        Team *cur = teamHead;
        while (cur->next) cur = cur->next;
        cur->next = team;
    }
    return team;
}

void buildIndex() {
    Team *cur = teamHead;
    int n = 0;
    while (cur) {
        teamIdx[n++] = cur;
        cur = cur->next;
    }
    for (int i = 1; i < n; i++) {
        Team *key = teamIdx[i];
        int j = i - 1;
        while (j >= 0 && teamIdx[j]->id > key->id) {
            teamIdx[j + 1] = teamIdx[j];
            j--;
        }
        teamIdx[j + 1] = key;
    }
}

Team *findTeam(int id) {
    int low = 0, high = 9;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (teamIdx[mid]->id == id) return teamIdx[mid];
        else if (teamIdx[mid]->id > id) high = mid - 1;
        else low = mid + 1;
    }
    return NULL;
}

int playerExists(int id) {
    for (Team *t = teamHead; t; t = t->next) {
        for (int i = 1; i <= 3; i++) {
            PlayerNode *cur = t->head[i];
            while (cur) {
                if (cur->id == id) return 1;
                cur = cur->next;
            }
        }
    }
    return 0;
}

Role roleFromString(const char *s) {
    if (strcmp(s, "Batsman") == 0) return BATSMAN;
    if (strcmp(s, "Bowler") == 0) return BOWLER;
    return ALLROUNDER;
}

void insertSorted(PlayerNode **head, PlayerNode *n) {
    while (*head && (*head)->pi >= n->pi) head = &(*head)->next;
    n->next = *head;
    *head = n;
}

int addPlayer(Team *t, PlayerNode *p, int checkRange) {
    if (checkRange && (p->id < 1 || p->id > 1000)) {
        printf("Player ID must be between 1 and 1000.\n");
        free(p);
        return 0;
    }
    if (playerExists(p->id)) {
        printf("Player with ID %d already exists.\n", p->id);
        free(p);
        return 0;
    }
    if (t->total >= 50) {
        printf("Team %s has reached the maximum number of players.\n", t->name);
        free(p);
        return 0;
    }
    p->pi = calcPi(p);
    insertSorted(&t->head[p->role], p);
    t->total++;
    if (p->role != BOWLER) {
        t->srSum += p->sr;
        t->srCount++;
    }
    return 1;
}

void loadData() {
    for (int i = 0; i < teamCount; i++) {
        createTeam(i + 1, teams[i]);
    }
    buildIndex();

    for (int i = 0; i < playerCount; i++) {
        const Player *h = &players[i];

        Team *t = NULL;
        for (Team *cur = teamHead; cur; cur = cur->next) {
            if (strcmp(cur->name, h->team) == 0) { t = cur; break; }
        }
        if (!t) continue;

        PlayerNode *p = (PlayerNode *)calloc(1, sizeof(PlayerNode));
        p->id = h->id;
        strncpy(p->name, h->name, 50);
        strncpy(p->team, h->team, 50);
        p->role = roleFromString(h->role);
        p->runs = h->totalRuns;
        p->avg = h->battingAverage;
        p->sr = h->strikeRate;
        p->wickets = h->wickets;
        p->er = h->economyRate;

        addPlayer(t, p, 0);
    }
}

/* ---------- display ---------- */
void printHeader(int showTeam) {
    if (showTeam) printf("%-5s %-24s %-14s %-12s %6s %6s %6s %5s %5s %8s\n", "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Idx");
    else          printf("%-5s %-24s %-12s %6s %6s %6s %5s %5s %8s\n", "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Idx");
}

void printPlayer(const PlayerNode *p, int showTeam) {
    const char *r = (p->role == BATSMAN) ? "Batsman" : (p->role == BOWLER) ? "Bowler" : "All-rounder";
    printf("%-5d %-24s ", p->id, p->name);
    if (showTeam) printf("%-14s ", p->team);
    printf("%-12s %6d %6.1f %6.1f %5d %5.1f %8.2f\n", r, p->runs, p->avg, p->sr, p->wickets, p->er, p->pi);
}

void displayTeam(const Team *t) {
    printf("Players of Team %s:\n", t->name);
    printHeader(0);
    for (int i = 1; i <= 3; i++)
        for (PlayerNode *cur = t->head[i]; cur; cur = cur->next)
            printPlayer(cur, 0);
    printf("Total Players: %d\n", t->total);
    printf("Average Batting Strike Rate: %.2f\n", t->srCount ? t->srSum / t->srCount : 0.0);
}

static float teamAvgSR(const Team *t) {
    return t->srCount ? t->srSum / t->srCount : 0.0f;
}

void displayTeamsBySR() {
    Team *arr[10];
    int n = 0;
    for (Team *cur = teamHead; cur; cur = cur->next) arr[n++] = cur;

    for (int i = 0; i < n - 1; i++) {
        int max = i;
        for (int j = i + 1; j < n; j++) {
            if (teamAvgSR(arr[j]) > teamAvgSR(arr[max])) max = j;
        }
        Team *temp = arr[i];
        arr[i] = arr[max];
        arr[max] = temp;
    }

    printf("Teams Sorted by Average Batting Strike Rate\n");
    printf("%-5s %-14s %10s %14s\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
    for (int i = 0; i < n; i++)
        printf("%-5d %-14s %10.2f %14d\n", arr[i]->id, arr[i]->name, teamAvgSR(arr[i]), arr[i]->total);
}

void displayTopK(Team *t, Role role, int K) {
    if (K <= 0) {
        printf("K must be greater than 0.\n");
        return;
    }
    PlayerNode *cur = t->head[role];
    if (!cur) {
        printf("No players of this role in team %s.\n", t->name);
        return;
    }
    printf("Top %d players of Team %s:\n", K, t->name);
    printHeader(0);
    int count = 0;
    while (cur && count < K) {
        printPlayer(cur, 0);
        cur = cur->next;
        count++;
    }
}

/* ---------- option 5: merge sorted lists ---------- */
PlayerNode *cloneList(PlayerNode *head) {
    PlayerNode *newHead = NULL, *tail = NULL;
    while (head) {
        PlayerNode *n = (PlayerNode *)malloc(sizeof(PlayerNode));
        *n = *head;
        n->next = NULL;
        if (!newHead) newHead = n;
        else tail->next = n;
        tail = n;
        head = head->next;
    }
    return newHead;
}

PlayerNode *mergeTwo(PlayerNode *a, PlayerNode *b) {
    PlayerNode dummy, *tail = &dummy;
    dummy.next = NULL;
    while (a && b) {
        if (a->pi >= b->pi) { tail->next = a; a = a->next; }
        else                { tail->next = b; b = b->next; }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    return dummy.next;
}

void displayRoleAllTeams(Role role) {
    PlayerNode *lists[10];
    int n = 0;
    for (Team *t = teamHead; t; t = t->next)
        lists[n++] = cloneList(t->head[role]);

    while (n > 1) {
        int m = 0;
        for (int i = 0; i + 1 < n; i += 2)
            lists[m++] = mergeTwo(lists[i], lists[i + 1]);
        if (n % 2) lists[m++] = lists[n - 1];
        n = m;
    }

    PlayerNode *result = lists[0];
    if (!result) {
        printf("No players of this role.\n");
        return;
    }
    const char *r = (role == BATSMAN) ? "Batsmen" : (role == BOWLER) ? "Bowlers" : "All-rounders";
    printf("%s of all teams:\n", r);
    printHeader(1);
    for (PlayerNode *cur = result; cur; cur = cur->next)
        printPlayer(cur, 1);

    while (result) {
        PlayerNode *temp = result;
        result = result->next;
        free(temp);
    }
}

/* ---------- option 1 ---------- */
void addPlayerMenu() {
    int teamId;
    printf("Enter Team ID to add player: ");
    if (!readInt(&teamId)) { printf("Invalid input.\n"); return; }
    Team *t = findTeam(teamId);
    if (!t) { printf("Team not found.\n"); return; }

    PlayerNode *p = (PlayerNode *)calloc(1, sizeof(PlayerNode));
    int role;
    char buf[256];

    printf("Enter Player Details:\n");
    printf("Player ID: ");
    if (!readInt(&p->id)) { printf("Invalid input.\n"); free(p); return; }

    printf("Name: ");
    if (!fgets(buf, sizeof(buf), stdin)) { free(p); return; }
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) < 1 || strlen(buf) > 50) {
        printf("Name length must be 1 to 50.\n");
        free(p);
        return;
    }
    strcpy(p->name, buf);

    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    if (!readInt(&role) || role < 1 || role > 3) {
        printf("Invalid role.\n");
        free(p);
        return;
    }
    p->role = (Role)role;

    printf("Total Runs: ");
    if (!readInt(&p->runs)) { printf("Invalid input.\n"); free(p); return; }
    printf("Batting Average: ");
    if (!readFloat(&p->avg)) { printf("Invalid input.\n"); free(p); return; }
    printf("Strike Rate: ");
    if (!readFloat(&p->sr)) { printf("Invalid input.\n"); free(p); return; }
    printf("Wickets: ");
    if (!readInt(&p->wickets)) { printf("Invalid input.\n"); free(p); return; }
    printf("Economy Rate: ");
    if (!readFloat(&p->er)) { printf("Invalid input.\n"); free(p); return; }

    strcpy(p->team, t->name);
    if (addPlayer(t, p, 1))
        printf("Player added successfully to Team %s!\n", t->name);
}

/* ---------- cleanup ---------- */
void freeAll() {
    Team *t = teamHead;
    while (t) {
        for (int i = 1; i <= 3; i++) {
            PlayerNode *cur = t->head[i];
            while (cur) {
                PlayerNode *temp = cur;
                cur = cur->next;
                free(temp);
            }
        }
        Team *nextTeam = t->next;
        free(t);
        t = nextTeam;
    }
    teamHead = NULL;
}

/* ---------- main ---------- */
int main() {
    loadData();

    int choice = 0;
    do {
        printf("\n==============================================================================\n");
        printf("ICC ODI Player Performance Analyzer\n");
        printf("==============================================================================\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display all Players of specific role Across All Teams by performance index\n");
        printf("6. Exit\n");
        printf("==============================================================================\n");
        printf("Enter your choice: ");

        if (!readInt(&choice)) { choice = 0; }

        if (choice == 1) {
            addPlayerMenu();
        } else if (choice == 2) {
            int id;
            printf("Enter Team ID: ");
            if (!readInt(&id)) { printf("Invalid input.\n"); continue; }
            Team *t = findTeam(id);
            if (!t) printf("Team not found.\n");
            else displayTeam(t);
        } else if (choice == 3) {
            displayTeamsBySR();
        } else if (choice == 4) {
            int id, r, k;
            printf("Enter Team ID: ");
            if (!readInt(&id)) { printf("Invalid input.\n"); continue; }
            Team *t = findTeam(id);
            if (!t) { printf("Team not found.\n"); continue; }
            printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
            if (!readInt(&r) || r < 1 || r > 3) { printf("Invalid role.\n"); continue; }
            printf("Enter number of players: ");
            if (!readInt(&k)) { printf("Invalid input.\n"); continue; }
            displayTopK(t, (Role)r, k);
        } else if (choice == 5) {
            int r;
            printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
            if (!readInt(&r) || r < 1 || r > 3) { printf("Invalid role.\n"); continue; }
            displayRoleAllTeams((Role)r);
        } else if (choice != 6) {
            printf("Invalid choice.\n");
        }
    } while (choice != 6);

    freeAll();
    return 0;
}