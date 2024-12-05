#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define maxSize 1000
#define specieCountMaxSize 10

typedef struct{
    float sepalLength, sepalWidth;
    float petalLength, petalWidth;
    char specie[32];
}flor;

typedef struct{
    flor* florPointer;
    float distance;
}nearestInfo;

typedef struct{
    char specie[32];
    int counter;
}specieCount;

nearestInfo *createKNearestVet(int k){
    nearestInfo *kNearestVet = malloc(sizeof(nearestInfo)*k);
    return kNearestVet;
}

float distanciaEuclidiana(flor *flor1,flor *flor2){
    float d;
    float sepalLength, sepalWidth;
    float petalLength, petalWidth;

    sepalLength = (flor1->sepalLength - flor2->sepalLength) * (flor1->sepalLength - flor2->sepalLength);
    sepalWidth = (flor1->sepalWidth - flor2->sepalWidth) * (flor1->sepalWidth - flor2->sepalWidth);
    petalLength = (flor1->petalLength - flor2->petalLength) * (flor1->petalLength - flor2->petalLength);
    petalWidth = (flor1->petalWidth - flor2->petalWidth) * (flor1->petalWidth - flor2->petalWidth);
    d = sqrt(sepalLength+sepalWidth+petalLength+petalWidth);
    return d;
}

int readIrisData(char *argv1,flor *vetFlores, int *lenVetFlores){
    FILE *fd = fopen(argv1,"r");
    if(fd==NULL){
        return -1;
    }

    char c;
    while(c!='\n'){
        c=fgetc(fd);
    }

    while(fscanf(fd,"%f",&vetFlores[*lenVetFlores].sepalLength)==1){
        fgetc(fd);
        fscanf(fd,"%f",&vetFlores[*lenVetFlores].sepalWidth);
        fgetc(fd);
        fscanf(fd,"%f",&vetFlores[*lenVetFlores].petalLength);
        fgetc(fd);
        fscanf(fd,"%f",&vetFlores[*lenVetFlores].petalWidth);
        fgetc(fd);
        int j=0;
        while((c=fgetc(fd))!='\n'){
            vetFlores[*lenVetFlores].specie[j++]=c;
        }
        vetFlores[*lenVetFlores].specie[j]='\0';
        (*lenVetFlores)++;
        if(*lenVetFlores>=maxSize){
            fclose(fd);
            return -1;
        }
    }
    fclose(fd);
    return 0;
}

int readNewFile(char *argv2,flor *vetFlores, int *lenVetFlores,int *oldLenVetFlores){
    FILE *newFd = fopen(argv2,"r");
    if(newFd==NULL){
        return -1;
    }

    int counter=0;
    while(fscanf(newFd,"%f",&vetFlores[*lenVetFlores].sepalLength)==1){
        fgetc(newFd);
        fscanf(newFd,"%f",&vetFlores[*lenVetFlores].sepalWidth);
        fgetc(newFd);
        fscanf(newFd,"%f",&vetFlores[*lenVetFlores].petalLength);
        fgetc(newFd);
        fscanf(newFd,"%f",&vetFlores[*lenVetFlores].petalWidth);
        fgetc(newFd);
        (*lenVetFlores)++;
        counter++;
        if(*lenVetFlores>=maxSize){
            fclose(newFd);
            return -1;
        }
    }

    *oldLenVetFlores = (*lenVetFlores)-counter;
    fclose(newFd);
    return 0;
}

void calculateNearest(flor *vetFlores,int position,nearestInfo *kNearestVet,int k){
    int lenKNearestVet=0;
    for(int j=0;j<position;j++){
        float distance = distanciaEuclidiana(&vetFlores[j],&vetFlores[position]);
        if(lenKNearestVet<k){
            kNearestVet[lenKNearestVet].florPointer=&vetFlores[j];
            kNearestVet[lenKNearestVet].distance=distance;
            lenKNearestVet++;
        }else{
            int maior = 0, indexMaior;
            for(int i=0;i<k;i++){
                if(kNearestVet[i].distance>maior){
                    indexMaior = i;
                }
            }
            if(distance<kNearestVet[indexMaior].distance){
                kNearestVet[indexMaior].florPointer=&vetFlores[j];
                kNearestVet[indexMaior].distance=distance;
            }
        }
    }
}

void setSpecie(flor *vetFlores,int position,nearestInfo *kNearestVet,int k,char *specie){
    specieCount *vetSpecies = malloc(sizeof(specieCount)*specieCountMaxSize);
    int lenVetSpecies=0;

    for(int i=0;i<k;i++){
        int flag=0;
        for(int j=0;j<lenVetSpecies;j++){
            if(strcmp(kNearestVet[i].florPointer->specie,vetSpecies[j].specie)==0){
                (vetSpecies[j].counter++);
                flag=1;
            }
        }
        if(flag==0){
            strcpy(vetSpecies[lenVetSpecies].specie,kNearestVet[i].florPointer->specie);
            vetSpecies[lenVetSpecies++].counter=1;
        }
    }

    int maior=0, indexMaior;
    for(int i=0;i<lenVetSpecies;i++){
        if(vetSpecies[i].counter>maior){
            maior = vetSpecies[i].counter;
            indexMaior = i;
        }
    }

    strcpy(specie,vetSpecies[indexMaior].specie);
    free(vetSpecies);
}

void discoverSpecies(flor *vetFlores,int lenVetFlores,int oldLenVetFlores,int k){
    nearestInfo *kNearestVet = createKNearestVet(k);

    for(int i=oldLenVetFlores;i<lenVetFlores;i++){
        calculateNearest(vetFlores,i,kNearestVet,k);
        char specie[32];
        setSpecie(vetFlores,i,kNearestVet,k,specie);
        strcpy(vetFlores[i].specie,specie);
    }

    free(kNearestVet);
}

void printVet(flor *vetFlores,int lenVetFlores){
    for(int i=0;i<lenVetFlores;i++){
        printf("%.1f %.1f %.1f %.1f %s\n",vetFlores[i].sepalLength,vetFlores[i].sepalWidth,vetFlores[i].petalLength,vetFlores[i].petalWidth,vetFlores[i].specie);
    }
}

int writeNewFile(char *argv2,flor *vetFlores,int lenVetFlores,int oldLenVetFlores){
    FILE *newFd = fopen(argv2,"w");
    if(newFd==NULL){
        return -1;
    }

    for(int i=oldLenVetFlores;i<lenVetFlores;i++){
        fprintf(newFd,"%.1f,",vetFlores[i].sepalLength);
        fprintf(newFd,"%.1f,",vetFlores[i].sepalWidth);
        fprintf(newFd,"%.1f,",vetFlores[i].petalLength);
        fprintf(newFd,"%.1f,",vetFlores[i].petalWidth);
        fprintf(newFd,"%s\n",vetFlores[i].specie);
    }
    
    fclose(newFd);
    return 0;
}

int updateIrisData(char *argv1,flor *vetFlores,int lenVetFlores,int oldLenVetFlores){
    FILE *fd = fopen(argv1,"a");
    if(fd==NULL){
        return -1;
    }

    for(int i=oldLenVetFlores;i<lenVetFlores;i++){
    fprintf(fd,"%.1f,",vetFlores[i].sepalLength);
    fprintf(fd,"%.1f,",vetFlores[i].sepalWidth);
    fprintf(fd,"%.1f,",vetFlores[i].petalLength);
    fprintf(fd,"%.1f,",vetFlores[i].petalWidth);
    fprintf(fd,"%s\n",vetFlores[i].specie);
    }

    fclose(fd);
    return 0;
}

int main(int argc,char *argv[]){
    flor *vetFlores = malloc(sizeof(flor)*maxSize);
    int lenVetFlores=0, oldLenVetFlores;
    int k;

    int res1 = readIrisData(argv[1],vetFlores,&lenVetFlores);
    if(res1==-1){
        printf("Erro ao ler o primeiro arquivo.\n");
        return -1;
    }

    printf("K:");
    scanf("%d",&k);

    int res2 = readNewFile(argv[2],vetFlores,&lenVetFlores,&oldLenVetFlores);
    if(res2==-1){
        printf("Erro ao ler o segundo arquivo.\n");
        return -1;
    }

    discoverSpecies(vetFlores,lenVetFlores,oldLenVetFlores,k);
    writeNewFile(argv[2],vetFlores,lenVetFlores,oldLenVetFlores);

    int update;
    printf("Would you like to update the Iris Data? (0/1)");
    scanf("%d",&update);
    if(update==1){
        updateIrisData(argv[1],vetFlores,lenVetFlores,oldLenVetFlores);
        printf("Successfully updated!\n");
    }
    
    free(vetFlores);
    return 0;
}