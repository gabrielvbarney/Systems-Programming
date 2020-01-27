#include "parseline.h"

int countStages(char *cmdline){
    char* token;
    int stages = 0;

    if((token = strtok(cmdline, "|")) == NULL){
        return stages;
    }
    stages++;
    while((token = strtok(NULL, "|")) != 0){
        stages++;
    }
    if(stages > 9){
        fprintf(stderr, "too many stages\n");
        exit(1);
    }


    return stages;
}

void stageList(char* cmdline, char** buffarr){
    char *token;
    int i = 0;
    if((token = strtok(cmdline, "|")) != NULL){
        buffarr[i] = (char*)malloc(strlen(token)+1);
        strcpy(buffarr[i++], token);
    }
    while((token = strtok(NULL, "|"))!=0){
        buffarr[i]= (char*)malloc(strlen(token)+1);
        strcpy(buffarr[i++], token);
    }

}

void processPipe(char *cmdline, Pipeline pipeline){
    int total = 0;
    char* line2;
    char* temptoken; /* need to free */ 
    char** buffarr;
    int i = 0;

    line2 = strdup(cmdline);

    total = countStages(cmdline);


    if(total > 9){
        fprintf(stderr, "too many stages\n");
        exit(1);
    }

    buffarr = (char**)malloc(sizeof(char*) * 11); /* extra one for safety*/
    stageList(line2, buffarr);
    free(line2);
    
    if(total == 1){
        /* This means we only have a signle stage bc no '|' were found*/
        processSingle(buffarr[0]);
    }

    else{
    /* Break into stages and then process each stage to get all the info*/
        pipeline.numStages = 0;
        for(i = 0; i < total ; i++){
            temptoken = strdup(buffarr[i]);
            processStage(temptoken, i,total);
            free(temptoken);
            pipeline.numStages++;
        }
        for(i = 0; i < total; i++){
            free(buffarr[i]);
        }
    }
    free(buffarr);
    
}
void processSingle(char *stage){
    char out[] = "original stdout";
    char in[]  = "original stdin";
    char *temp;
    char *temp2;
    char *args;
    char argDelim[] = " ><";
    Parser *parser = (Parser*)malloc(sizeof(Parser));

    strcpy(parser->stageName, stage);

    temp = strdup(stage);
    temp2 = strdup(stage);


    if((strchr(stage, '<') != NULL) && (strchr(stage, '>') != NULL)){
    /* the fun case, we have both! */
        temp = strdup(stage);
        temp2 = strdup(stage);
        getOutput(temp2, parser);
        getInput(temp, parser);
        free(temp);
        temp  = strdup(stage);
        getArgs(parser, temp);                
    }
              
    else if(!strchr(stage, '<') && !strchr(stage, '>')){
    /* This means we only have args, stdin, and stdout */
         strcpy(parser->input, in);
         strcpy(parser->output, out);
         getArgs(parser, temp);
     }
     else if(!strchr(stage, '<') && (strchr(stage, '>') != NULL)){
     /* we only have to deal with output */
         getOutput(temp2, parser);
         strcpy(parser->input, in);
         getArgs(parser, temp);
     }
     else if((strchr(stage, '<') != NULL) && !strchr(stage, '>')){
         getInput(temp, parser);
         strcpy(parser->output, out);
         getArgs(parser, temp2);
        
    }
    printStage(parser, 0);
    free(temp);
    free(temp2);


    
}


void getArgs(Parser *parser, char* stage){
    char* args;
    char argDelim[] = " ><";
    args = strtok(stage, argDelim);
    parser->_argc = 0;
    while(args != NULL){
        if((strcmp(args, parser->input) != 0) &&
                (strcmp(args, parser->output)) != 0){
            parser->_argv[parser->_argc] = (char*)malloc(strlen(args)+1);
            strcpy(parser->_argv[parser->_argc++], args);
        }
        args = strtok(NULL, argDelim);
        
    }
}


Parser *processStage(char *token, int num, int total){
    char out[] = "original stdout";
    char in[]  = "original stdin";
    char pipe_in[]  = "pipe from stage ";
    char pipe_out[] = "pipe to stage ";

    Parser *parser = (Parser*)malloc(sizeof(Parser));
    char stagestr[3] = {'\0'};
    char *args;
    char argDelim[] = " ><";
    char buffer[512] = {'\0'};
    char *temptoken = strdup(token);
    char *temptoken2 = strdup(token);
   
  
    /* get the name of the stage */
    strcpy(parser->stageName, token);

    /*  getCmds(temptoken, parser);*/  
 /*   free(temptoken); * free current temp */
    args = strtok(temptoken, argDelim);
    parser->_argc = 0;
    while(args != NULL){
    /* get and store the args for the stage and increment _argc*/
        parser->_argv[parser->_argc] = (char*)malloc(strlen(args)+1);
        strcpy(parser->_argv[parser->_argc++], args);
        args = strtok(NULL, argDelim);
    }

    /* We have all args & cmds,now lets deal w/ all the diff iputs & outputs*/
    if(!strchr(token, '>') && !strchr(token, '<')){
        /* case with have no commands, only for Pipes with > 1 stages */
        if(num == (total-1)){
            /* if we have only args and no cmds for the last stage */
            int_to_ascii((num-1), stagestr);
            strcpy(buffer, pipe_in);
            strcat(buffer, stagestr);
            strncpy(parser->input, buffer, strlen(buffer));
            memset(buffer, 0, 512);
           /* parser->output = (char *)malloc(strlen(STDOUT));*/
            strcpy(parser->output, out);
        }
        else if(num == 0){
            /* first stage, no args, in a multi-stage Pipeline*/
            strcpy(parser->input, in);
            int_to_ascii(num + 1, stagestr);
            strcpy(buffer, pipe_out);
            strcat(buffer, stagestr);
            strncpy(parser->output, buffer, strlen(buffer)+1);
            memset(buffer, 0, 512);
         /*   strcpy(parser->output, strcat(PIPE_OUTPUT, stagestr));*/
        }
        else{
            /* No cmds, middle stage, in from pipe and out to pipe*/
            int_to_ascii(num-1, stagestr);
            strcpy(buffer, pipe_in);
            strcat(buffer, stagestr);
            strcpy(parser->input, buffer);
            memset(buffer, 0, 512);
            int_to_ascii(num+1, stagestr);
            strcpy(buffer, pipe_out);
            strcat(buffer, stagestr);
            strncpy(parser->output, buffer, strlen(buffer));
            memset(buffer, 0, 512);
        }
    }
    else if((!strchr(token, '>')) && (strchr(token, '<') != NULL)){
        /* We have a stage with only input redirect. This means that it has 
         * to be the first stage or else there would be an error */
        getInput(temptoken2, parser);
        int_to_ascii(num, stagestr);
        strcpy(buffer, pipe_out);
        strcat(buffer, stagestr);
        strncpy(parser->output, buffer, strlen(buffer));
        memset(buffer, 0, 512);
    }
    else if((strchr(token, '>') != NULL) && (!strchr(token, '<'))){
        /* We have only output redirect which means we have to be at the 
         * last stage
         * or else there would be an error, cant have a > b | c (two outputs)*/
        getOutput(temptoken2, parser);
        int_to_ascii((num-1), stagestr);
        strcpy(buffer, pipe_in);
        strcat(buffer, stagestr);
        strcpy(parser->input, buffer);
        memset(buffer, 0, 512);
    }
    printStage(parser, num);
    return parser;
}


void getCmds(char* token, Parser *parser){
    int i;
    int j = 0;
    for(i = 0; i < strlen(token); i++){
        if(token[i] == '>'|| token[i] == '<'){
            parser->cmds[j++] = &token[i];
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


void int_to_ascii(int stageNum, char* str){
    if(stageNum < 10){
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


void printStage(Parser *parser, int num){
        int i = 0;
        printf("\n--------\nStage %d: \"%s\"\n--------\n",
               num, parser->stageName);
        printf("\t input: %s\n", parser->input);
   /*     printf("\toutput: %s\n", parser->output);*/
        printf("\toutput: %s\n", parser->output);
        printf("\t  argc: %d\n", parser->_argc);
        printf("\t  argv: ");
        for (i = 0; i < parser->_argc; i++) {
            printf("\"%s\"", parser->_argv[i]);
            if((i + 1) < parser->_argc){
                printf(",");
            }
        }
        printf("\n");
}


int errorProcess(char *cmd_line) {
    int i = 0;
    int in_count = 0;
    int full_in_count = 0;
    int out_count = 0;
    int full_out_count = 0;
    int pipe_count = 0;
    int full_pipe_count = 0;
    int normal_count = 0;
    int full_normal_count = 0;
    int space_count = 0;
    char prevbuff[1];
    char *templine;
    
    
    templine = strdup(cmd_line);
    free(templine);
    templine = strdup(cmd_line);
    if(strtok(templine, "\n  ") == NULL){
        fprintf(stderr, "No args");
        exit(1);
    }
    
    free(templine);
    
    for (i = 0; i < strlen(cmd_line); i++) {
        if ((int)cmd_line[i] == SPACE_ASCII) {
            space_count += 1;
        }
        if (space_count == 2 || !pipe_count) {
            if ((int)cmd_line[i] != IN_ASCII) {
                if ((int)cmd_line[i] != OUT_ASCII) {
                    if ((int)cmd_line[i] != PIPE_ASCII) {
                        if ((int)cmd_line[i] != SPACE_ASCII) {
                            if ((int)prevbuff[0] != IN_ASCII && 
                                     (int)prevbuff[0] != OUT_ASCII) {
                               if (i == 0 || (int)prevbuff[0] == SPACE_ASCII) {
                                    normal_count += 1;
                                    full_normal_count += 1;
                                    space_count = 0;
                               }
                            }
                        }
                    }
                }
            }
        }



        if ((int)cmd_line[i] == IN_ASCII) {
            in_count += 1;
            full_in_count += 1;
        } else if ((int)cmd_line[i] == OUT_ASCII) {
            out_count += 1;
            full_out_count += 1;
        } else if ((int)cmd_line[i] == PIPE_ASCII) {
            pipe_count += 1;
            full_pipe_count += 1;
        }
        if (!pipe_count) {
            if (in_count == 2 || (in_count == 1 && !normal_count)) {
                fprintf(stderr, "cmd: bad input redirection\n");
                return 1;
            } else if (out_count == 2 || (out_count == 1 && !normal_count)) {
                fprintf(stderr, "cmd: bad output redirection\n");
                return 1;
            }
        } if (pipe_count == 1 && !normal_count) {
            fprintf(stderr, "invalid null command\n");
            return 1;
        } if (full_out_count == 1 &&
              (full_pipe_count == 2 ||
               (full_normal_count >= 3 && normal_count >= 1))) {
            if (in_count != out_count || normal_count != 3) {
                fprintf(stderr, "more: ambiguous output\n");
                return 1;
            }
        } if (full_in_count == 1 &&
              (full_pipe_count == 2 ||
               (full_normal_count >= 3 && normal_count >= 1))) {
            if (out_count != in_count || normal_count != 3) {
             /*   printf("in: %d, out: %d, norm: %d\n",
                       in_count, out_count, normal_count);*/
                fprintf(stderr, "more: ambiguous input\n");
                return 1;
            }
        }
        if (pipe_count) {
            in_count = 0;
            out_count = 0;
            pipe_count = 0;
            normal_count = 0;
        }
        memset(prevbuff, 1, 1);
        prevbuff[0] = cmd_line[i];
    }
    return 0;
}



int main(int argc, const char *argv[]) {
    Pipeline pipeline;
    char cmd_line[512] = {'\0'};
    printf("line: ");
    scanf("%[^\n]", cmd_line);
    if(errorProcess(cmd_line) == 1){
        return 1;
    }
    pipeline.numStages = 0;
    processPipe(cmd_line, pipeline);


    if (strlen(cmd_line) > MAX_CMD_LINE) {
        fprintf(stderr, "command too long\n");
        exit(1);
    }

    return 0;
}





