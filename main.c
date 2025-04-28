#include <stdio.h>
#include <git2.h>

void print_sumn()
{
    printf("lole.");
}



void get_commit_info(git_commit* commit) //should work hopefully i give up for now
{
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
    printf("author: %s %s", author->name, author->email);
    printf("commit time (ur time): %ld", our_commit_time);
} 

int main()
{
    git_libgit2_init();
    print_sumn();
    git_repository* repo = NULL;
    git_signature* me = NULL;
    git_oid new_commit_id = 0;
    git_commit* commit;
    
    //i dont understand
    int error = git_signature_now(&me,"ME","example@gmail.com");
    error = git_repository_init(&repo,"/home/vallislfc/Downloads",1);
    error=git_commit_create(&new_commit_id, repo, "HEAD", me, me, "UTF-8","commit msg test", free,2,parents);
    error = git_commit_lookup(&commit, repo, &new_commit_id);
    get_commit_info(commit);
    return error;
}