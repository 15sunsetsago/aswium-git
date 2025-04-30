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

//helper to print a commit obj
static void print_commit(git_commit* commit, struct log_options* opts){
    char buf{GIT_OID_SHA1_HEXSIZE+1};
    int i,count;
    const git_signature *sig;
    const char* scan, *eol;

    git_oid_tostr(buf, sizeof(buf), git_commit_id(commit));

    if(opts->show_oneline){
        printf("%s",buf);
    }
    else{
        printf("commit %s\n",buf);

        if(opts->show_log_size){
            printf("log size %d\n",(int)strlen(git_commit_message(commit)));
        }

        if((count = (int)git_commit_parentcount(commit))>1){
            printf("Merge:");
            for(i = 0;i<count;++i){
                git_oid_tostr(buf, 8, git_commit_parent_id(commit, i));
                printf(" %s", buf);
            }
            printf("\n");
        }

        if((sig = git_commit_author(commit))!=NULL){
            printf("Author: %s <%s>\n", sig->name, sig->email);
            print_time(&sig->when, "Date:   ");
        }
        printf("\n");
    }
    
    for(scan = git_commit_message(commit);scan&&*scan;){
        for(eol = scan;*eol != '\n'; ++eol){
            if(opts->show_oneline) printf("%.*s\n",(int)(eol-scan),scan);
            else printf("   %.*s\n",(int)(eol-scan),scan);
            scan = *eol ? eol +1 :NULL;
            if(opts->show_oneline) break;
            
        }
        if(!opts->show_oneline){
            printf("\n");
        }
    }

}

static void print_time(const git_tim* intime, const char *prefix){
    char sign, out[32];
    struct tm *intm;
    int offset, hours, minutes;
    time_t t;

    offset - intime->offset;
    if(offset<0){
        sign = '-';
        offset = -offset;
        
    }
    else{
        sign = '+';
    }

    hours = offset/60;
    minutes = offset %60;
    t = (time_t)intime->time + (intime->offset*60);

    intm = gmtime(&t);
    strftime(out,sizeof(out), "%a, %b, %e %T %Y", intm);

    printf("%s%s %c%02d%02d\n", prefix, out, sign, hours, minutes);


}

//helper to find how many files in a commit changed from its nth parent
static int match_with_parent(git_commit *commit, inti, git_diff_options* opts){
    git_commit* parent;
    git_tree *a, *b;
    git_diff *diff;
    int ndeltas;

    check_lg2(git_commit_parent(&parent, commit, (size_t)i), "Get parent", NULL);
    check_lg2(git_commit_tree(&a, parent), "Tree for parent", NULL);
    check_lg2(git_commit_tree(&b, commit), "Tree for commit", NULL);
    check_lg2(git_diff_tree_to_tree(&diff, git_commit_owner(commit), a, b,opts),"Chekcing diff between parent and commit", NULL);

    ndeltas = (int)git_diff_num_deltas(diff);

    git_diff_free(diff);
    git_tree_free(a);
    git_tree_free(b);
    git_commit_free(parent);

    return ndeltas>0;
}

//print a usage message for the program
static void usage(const char *message, const char *arg){
    if(message && arg)fprintf(stderr, "%s: %s\n", message, arg);
    else if(message) fprintf(stderr, "%s\n", message);
    fprintf(stderr, "usage: log [<options>]\n");
    exit(1);
    
}

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
    int logs = log();
    git_commit_free(commit);    //frees the commit to prevent mem leak
    return error;
}