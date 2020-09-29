#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool isCycle(int loser, int starter);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // do a linear search over each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // ranks[0] holds preferred candidate ID over ranks[1] and over ranks[2] candidate ID
            // ranks[1] holds preferred candidate ID over ranks[2] candidate ID
            // ranks[n] holds preferred candidate ID over ranks[n + (1 … candidate_count - 1)]
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // skip over pair of same candidate
            if (i == j)
            {
                continue;
            }

            // candidate ID i won over candidate ID j
            if (preferences[i][j] > 0 && preferences[i][j] > preferences[j][i]) // add check who actually has won
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // do a selection sort DSC
    for (int i = 0; i < pair_count; i++)
    {
        int win = 0;
        int winMax = i;

        // don't touch leftmost items
        for (int p = i; p < pair_count; p++)
        {
            win = preferences[pairs[p].winner][pairs[p].loser];
            if (win > preferences[pairs[winMax].winner][pairs[winMax].loser])
            {
                winMax = p;
            }
        }

        // same values of votes don't matter, so skip them in the swapping process
        // compare current winMax to current item
        if (preferences[pairs[winMax].winner][pairs[winMax].loser] <= preferences[pairs[i].winner][pairs[i].loser])
        {
            continue;
        }

        // swap current item with winMax's item
        pair tmp = pairs[i];
        pairs[i] = pairs[winMax];
        pairs[winMax] = tmp;
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int p = 0; p < pair_count; p++)
    {
        int i = pairs[p].winner;
        int j = pairs[p].loser;

        // check if we create a cycle
        if (!isCycle(j, i))
        {
            locked[i][j] = true;

            printf("%s --> %s\n", candidates[i], candidates[j]);
        }
    }
    return;
}

// Check if we create a cycle with the next edge
// tracing from the soon to be loser forwards through their connections
// and checking if we are in the path of connections
bool isCycle(int loser, int starter)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i]) // connection from loser to someone else found
        {
            if (i == starter || isCycle(i, starter)) // the connection is back to us or we found ourselves in the next connection
            {
                return true;
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // do a linear search column wise
    for (int j = 0; j < pair_count; j++)
    {
        int edgeCount = 0;

        for (int i = 0; i < pair_count; i++)
        {
            // no one can point to themselves
            if (i == j)
            {
                continue;
            }

            // count edges pointing to j
            if (locked[i][j])
            {
                edgeCount++;
                break; // someone is pointing at us, no need to check further
            }
        }

        if (edgeCount == 0)
        {
            // no edges pointing in, this is the winner
            printf("%s\n", candidates[j]);
            return;
        }
    }

    return;
}
