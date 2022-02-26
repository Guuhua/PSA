#include "fasta.h"


size_t *countInfo(char *path, size_t *num)
{
    FILE *fp = NULL;
    fp = fopen(path, "r");
    if (fp == NULL) exit(EXIT_FAILURE);
    
    int state = 0;
    char *line = NULL;
    ssize_t read;
    size_t n = 0, length = 0, tempL = 1;
    size_t *lens = (size_t *)malloc(sizeof(size_t) * tempL * 100);
    if (lens == NULL) exit(EXIT_FAILURE);

    *num = 0;
    while ((read = getline(&line, &n, fp)) != -1)
    {
        if (line[0] == '>')
        {
            if (state == 1)
            {
                if ((*num) >= (tempL * 100))
                {
                    lens = realloc(lens, sizeof(size_t) * (++tempL) * 100);
                    if (lens == NULL) exit(EXIT_FAILURE);
                }
                lens[(*num)++] = length;
                length = 0;
            }
            state = 1;
        }
        else 
            length += (read - 1);
    }
    fclose(fp);
    if (line)
        free(line);
    
    lens = realloc(lens, sizeof(size_t) * ((*num) + 1));
    if (lens == NULL) exit(EXIT_FAILURE);
    lens[(*num)++] = length;
    
    return lens;
}


void readFasta(char *path, char ***strs, char ***labels, size_t *num)
{
    FILE *fp;
	char *line = NULL;
	size_t n = 0, idx = 0;
	ssize_t read;
    *num = 0;

    // get the number of and length of seq
    size_t *lens = (size_t *)countInfo(path, num);
    *strs   = (char **)malloc(sizeof(char *) * (*num));
    *labels = (char **)malloc(sizeof(char *) * (*num));

	fp = fopen(path, "r");
	if (fp == NULL) 
    {
        printf("Couldn't open the file %s\n", path);
        exit(EXIT_FAILURE);
    }
 
	int state = 0;
	while ((read = getline(&line, &n, fp)) != -1) {
		if (line[read - 1] == '\n')
			line[read - 1] = 0;
		if (line[0] == '>') 
        {
			if (state == 1)
				idx++;
			state = 1;
            (*strs)[idx] = (char *)malloc(sizeof(char) * (lens[idx] + 1));
            (*strs)[idx][0] = 0;
            (*labels)[idx] = (char *)malloc(sizeof(char) * read);
            strcpy((*labels)[idx], line+1);
		} 
        else 
            strcat((*strs)[idx], line);
	}
    free(lens);
    fclose(fp);
	if (line)
		free(line);
}


void writeFasta(char *path, char **strs, char **lables, size_t num)
{
    FILE *fp = fopen(path, "w");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    
    char *p = (char *)malloc(sizeof(char) * (LEN + 1));
    p[LEN] = 0;
    for (int i = 0; i < num; i++)
    {
        fputc('>', fp);
        fputs(lables[i], fp);
        fputc('\n', fp);
        int idx = 0, length = strlen(strs[i]);
        
        while (idx + LEN <= length)
        {
            strncpy(p, strs[i]+idx, LEN);
            fputs(p, fp);
            idx += LEN;
            fputc('\n', fp);
        }
        if (idx < length)
        {
            strcpy(p, strs[i]+idx);
            fputs(p, fp);
        }
        fputc('\n', fp);
    }
    free(p);
}