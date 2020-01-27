#include "parseline.h"

int countStages(char *cmdline){
    char* token;
    int stages = 0;

    if((token = strtok(cmdline, '|')) == NULL){
        return stages;
    }
    stages++;
    while((token = strtok(NULL, '|')) != 0){
        stages++;
    }

    return stages;    
}

void processPipe(char *cmdline, Pipeline *pipeline){
    int total;
    total = countStages(cmdline);
    
    pipeline->pipe = (Parser *)malloc(sizeof(Parser) * total);

    char *token = strtok(cmdline, "|"); /*points to the first token!*/ 
    if(!token){
    /* This means we only have a signle stage bc no '|' were found*/
        processSingle(cmdline, &(pipeline->pipe[0]));
    }

    /* Break into stages and then process each stage to get all the info*/
    while(token != NULL){
        processStage(token, &pipeline->pipe[pipeline->numStages],
                pipeline->numStages,total);
        pipeline->numStages++;
        token = strtok(NULL, "|"); /* get us the next token "Stage"*/
    }
}

void processStage(char *token, Parser *parser, int num, int total){
    int input;
    int output;
    char stagestr[3] = {'\0'};
   /* char* buffer[512] = {'\0'};*/
    char *args;
    char argDelim[] = " ><";
    int index = 0;
    char *temptoken; 
    
    /* get the name of the stage */
    strcpy(parser->stageName, token);

    temptoken = strdup(token);

    getCmds(temptoken, parser); /* get and store the commands for the stage*/
    free(temptoken);
    temptoken = strdup(token);
    args = strtok(temptoken, argDelim);
    parser->_argc = 0;
    while(args != NULL){
    /* get and store the args for the stage and increment _argc*/
        strcpy(parser->_argv[parser->_argc++], args);
        args = strtok(NULL, argDelim);
    } 

    /* We have all args & cmds,now lets deal w/ all the diff iputs & outputs*/
    if(!strchr(token, '>') && !strchr(token, '<')){
    /* case with have no commands, only for Pipes with > 1 stages */
        if(num == (total -1)){ 
        /* if we have only args and no cmds for the last stage */
            int_to_ascii((num-1), stagestr);
            strcpy(parser->input, strcat(PIPE_INPUT, stagestr));
            strcpy(parser->output, STDOUT);
        }
        else if(num == 0){
        /* first stage, no args, in a multi-stage Pipeline*/
            strcpy(parser->input, STDIN);
            int_to_ascii(num, stagestr);
            strcpy(parser->output, strcat(PIPE_OUTPUT, stagestr));
        }
        else{
        /* No cmds, middle stage, in from pipe and out to pipe*/
            int_to_ascii(num-1, stagestr);
            strcpy(parser->input,strcat(PIPE_INPUT, stagestr));
            int_to_ascii(num+1, stagestr);
            strcpy(parser->output, strcat(PIPE_OUTPUT, stagestr));
        }
    }
    else if((!strchr(token, '>')) && (strchr(token, '<') != NULL)){
    /* We have a stage with only input redirect. This means that it has to be
     * the first stage or else there would be an error */
        getInput(token, parser);
        int_to_ascii(num, stagestr);
        strcpy(parser->output, strcat(PIPE_OUTPUT, stagestr));
    }
    else if((strchr(token, '>') != NULL) && (!strchr(token, '<'))){
    /* We have only output redirect which means we have to be at the last stage
     * or else there would be an error, cant have a > b | c (two outputs)*/
        getOutput(token, parser);
        int_to_ascii((num-1), stagestr);
        strcpy(parser->input, strcat(PIPE_INPUT,stagestr));
    }    
}


void getCmds(char* token, Parser *parser){
    int i;
    int j = 0;
    for(i = 0; i < strlen(token); i++){
        if(token[i] == '>'|| token[i] == '<'){
            parser->cmds[j++] = token[i];
        }
    }
}

void getInput(char *token, Parser *parser){
    char *in1;
    char *in2;
    in1 = strtok(token, "<");
    in1 = strtok(NULL, "<");
    if((in2 = strtok(in1, " ")) == NULL){ /* is there more than one arg?*/
        strcpy(parser->input, in1);
    } 
    else{
        strcpy(parser->input, in2);
    }
}

void getOutput(char *token, Parser *parser){
    char *out1;
    char *out2;
    out1 = strtok(token, ">");
    out1 = strtok(NULL, ">");
    if((out2 = strtok(out1, " ")) == NULL){
       strcpy(parser->output, out1);
    }
    else{
        strcpy(parser->output, out2);
    }
}

void processSingle(char *cmdline, Parser *parser){
    /* strtok for >
     * strtok for < 
     * tokens
     */   
}

void int_to_ascii(int stageNum, char* str){
    if(stageNum > 10){
        str[0] = (char)(stageNum + 48);
        str[1] = '\0';
        str[2] = '\0';
    }
    else{
        str[0] = (char)((stageNum / 10) + 48);
        str[1] = (char)((stageNum % 10) + 48);
        str[2] = '\0';
    }
}

void printStages(Pipeline *pipeline){
    int i = 0; 
    for(i = 0; i < pipeline->numStages; i++)
}



int main(){

    /* sscanf in cmd line */
    /* check for format errors */
    /* processPipe */
    /* Print Pipe */
    /* ClearPipe */

    return 0;
}


