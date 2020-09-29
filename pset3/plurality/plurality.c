#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // do a linear search on each candidate --> no faster way, because we didn't sort and because we need to find the appropriate candidate.
    for (int i = 0; i < candidate_count; i++)
    {
        // if string name matches a name of the candidates[i].name
        if (strcmp(name, candidates[i].name) == 0)
        {
            // update candidates[i].votes
            candidates[i].votes++;
            return true;
        }
    }

    // name does not match any of the candidates[i].name
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // TODO: find ALL winners
    int max = 0;

    // find max votes by doing a selection sort of sorts
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > candidates[max].votes)
        {
            max = i;
        }
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == candidates[max].votes)
        {
            printf("%s\n", candidates[i].name);
        }
    }

    return;
}