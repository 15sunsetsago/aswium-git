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

int lg2_commit(git_repository* repo, int argc, char **argv){
    const char* opt = argv[1];
    const char* comment = argv[2];
    int error;

    git_oid commit_oid, tree_oid;
    git_tree* tree;
    git_index* index;
    git_object* parent = NULL;
    git_reference* ref = NULL;
    git_signature* author_signature, *committer_signature;

    //validate args
    if(argc<3||strcmp(opt,"-m")!=0){
        printf("USAGE: %s -m <comment>\n", argv[0]);
        return -1;
    }
    error = git_revparse_ext(&parent, &ref, repo, "HEAD");
    if(error == GIT_ENOTFOUND){
        printf("HEAD not found. Creating first commit\n");
        error =0;
    }
    else if(error!=0){
        const git_error* err = git_error_last();
        if(err)printf("ERROR %d: %s\n", err->klass,err->message);
        else printf("ERRO %d: no detailed info\n", error);
    }

    check_lg2(git_repository_index(&index, repo), "could not open repository index", NULL);
    check_lg2(git_index_write_tree(&tree_oid, index), "could not write tree", NULL);
    check_lg2(git_index_write(index), "could not write index", NULL);

    check_lg2(git_tree_lookup(&tree, repo, &tree_oid), "error looking up tree", NULL);

    check_lg2(git_signature_default_from_env(&author_signature, &committer_signature, repo),"Error creating signature", NULL);

    check_lg2(git_commit_create_v(&commid_oid, repo, "HEAD",author_signature, committer_signature, NULL, comment, tree. parent?1:0, parent), "error creating commit", NULL);

    git_index_free(index);
    git_signature_free(author_signature);
    git_signature_free(committer_signature);
    git_tree_free(tree);
    git_object_free(parent);
    git_reference_free(ref);
    return error;
}

int main()
{
    git_libgit2_init();
    print_sumn();
    
    return 0;
}