#include <stdio.h>
#include <git2.h>

void print_sumn()
{
    printf("lole.");
}

git_commit* getLastCommit(git_repository* repo) {   //ik this works since copied lol,. call git_commit_free when done
    int err;
    git_commit* commit = NULL;  //result;
    git_oid oid_parent_commit;  //the SHA1 for last commit;

    //resolve Head into SHA1
    err = git_reference_name_to_id(&oid_parent_commit, repo, "HEAD");
    if(err == 0){
        //get commit
        err = git_commit_lookup(&commit, repo, &oid_parent_commit);
        if(err ==0){
            return commit;
        }
    }
    return NULL;
}

int log(){
    diffopts.pathspec.strings;
    diffopts.pathspec.count;
    if(diffopts.pathspec.count>0){
        check_lg2(git_pathspec_new(&ps,&diffopts.pathspec),"Building pathspec", NULL);
    }
    for(;!git_revwalk_next(&old, this->walker);git_commit_free(commit)){
        check_lg2(git_commit_lookup(&commit, this->repo, &oid),"failed to look up commit", NULL);

        parents = (int)git_commit_parentcount(commit);
        if(diffopts.pathspec.count>0){
            int unmatched = parents;
            if(parents ==0){
                git_tree* tree;
                check_lg2(git_commit_tree(&tree,commit),"get tree", NULL);
                if(git_pathspec_match_tree(NULL, tree, GIT_PATHSPEC_NO_MATCH_ERROR,ps)!=0){
                    unmatched =1;
                    git_tree_free(tree);
                }
            }
        }
        print_commit(commit);
        if(show_diff){
        git_tree* a = NULL, *b =NULL;
        git_diff* diff = NULL;

        if(parents>1)continue;
        check_lg2(git_commit_tree(&b, commit),"Get tree", NULL);
        if(parents == 1){
            git_commit* parent;
            check_lg2(git_commit_parent(&parent, commit, 0),"get parent", NULL);
            check_lg2(git_commit_tree(&a, parent), "Tree for parent", NULL);
            git_commit_free(parent);
        }
        check_lg2(git_diff_tree_to_tree(&diff, git_commit_owner(commit),a,b,&diffopts), "DIff commit with parent", NULL);
        }
    }
    return 0;
}

void print_commit(git_commit* commit){
    git_commit_message(commit);
    const git_signature* sig = git_commit_author(commit);
    sig->name;
    sig->email;
    sig->when;
}

// void get_commit_info(git_commit* commit) //should work hopefully i give up for now
// {
//     git_libgit2_init();
//     const char* commit_msg;
//     const git_oid* commit_id = git_commit_id(commit);   //gets the commit id
//     commit_msg = git_commit_message(commit);    //gets the commit msg
//     const git_repository* repo = git_commit_owner(commit);    //gets the commit repo
//     git_time_t commit_time = git_commit_time(commit);   //gets the time commited
//     int time_zone_diff = git_commit_time_offset(commit);    //gets the time zone diff
//     int64_t our_commit_time = commit_time - time_zone_diff;     //updates the time so it will be our time instead of the commiters time
//     const git_signature* author = git_commit_author(commit);    //no idea how to get the stuff inside with no get functions 
//     printf("COMMIT id: %s", commit_id->id);
//     printf("commit msg: %s", commit_msg);
//     printf("repo: %s", git_repository_path(repo));
//     printf("author: %s %s", author->name, author->email);
//     printf("commit time (ur time): %ld", our_commit_time);
// } 

int main()
{
    git_libgit2_init();
    print_sumn();
    git_repository* repo = NULL;
    git_signature* me = NULL;
    git_oid new_commit_id = 0;
    git_commit* commit;
    
    //i dont understand how to intiate the comit with no tree and shi
    //ts pmo
    int error = git_signature_now(&me,"ME","example@gmail.com"); //set signature
    error = git_repository_init(&repo,"/home/vallislfc/Downloads",1);   //open new repo
    error=git_commit_create(&new_commit_id, repo, "HEAD", me, me, "UTF-8","commit msg test", free,2,parents);
    error = git_commit_lookup(&commit, repo, &new_commit_id);
    get_commit_info(commit);
    git_commit_free(commit);    //frees the commit to prevent mem leak
    return error;
}