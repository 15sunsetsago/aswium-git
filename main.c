#include <stdio.h>
#include "git2.h"

void print_sumn()
{
    printf("lole.");
}

void get_commit_info(const git_commit* commit){
    git_libgit2_init();
    const char* commit_msg;
    const git_oid* commit_id = git_commit_id(commit);   //gets the commit id
    commit_msg = git_commit_message(commit);    //gets the commit msg
    const git_repository* repo = git_commit_owner(commit);    //gets the commit repo
    git_time_t commit_time = git_commit_time(commit);   //gets the time commited
    int time_zone_diff = git_commit_time_offset(commit);    //gets the time zone diff
    int64_t our_commit_time = commit_time - time_zone_diff;     //updates the time so it will be our time instead of the commiters time
    const git_signature* author = git_commit_author(commit);    //no idea how to get the stuff inside with no get functions 
    printf("COMMIT id: %s", commit_id->id);
    printf("commit msg: %s", commit_msg);
    printf("repo: %s", git_repository_path(repo));
    



}
