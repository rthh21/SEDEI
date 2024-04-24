// gr. 161 - pb. 2 - STAN MARIUS ANDREI 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//----------------------------------------------------------------

int searchTerm(char **termeni, int s, int d, char *find) {
    if (s > d) {
        return 0; // Nu am gasit termenul
    }
    
    // Calcularea mijlocului intervalului
    int mid = (s + d) / 2;
    // Compararea termenului căutat cu termenul din mijlocul intervalului
    int compare = strcmp(termeni[mid], find);
    if (compare == 0) {
        return 1; // Termenul a fost găsit
    } else if (compare > 0) {
        // Caută în partea stângă a intervalului
        searchTerm(termeni, s, mid - 1, find);
        searchTerm(termeni, mid + 1, d, find);
    }
}

//----------------------------------------------------------------

int main(){
    char find[30];
    // deschidem fisierul de output
    FILE *index = fopen("index.out", "wb");
    if (index == NULL) {
        printf("Nu s-a putut deschide fisierul index.out!\n");
        return 1;
    }
    FILE *input = fopen("input.in", "r");
    if (input == NULL) {
        printf("Nu s-a putut deschide fisierul index.out!\n");
        return 1;
    }
    
    fgets(find, 30, input);
    // eliminam caracterul end line
    if(find[strlen(find)-1] == '\n'){
       find[strlen(find)-1] = '\0';
    }
    fprintf(index,"Termen: '%s'\n\nRezultat:\n",find);
    
    // dam start la timer - idee preloata de la timerul de la google 
    clock_t start, end;
    double time_sec;
    start = clock();
    
    // deschidem fisierul documente.txt
    FILE *doc = fopen("documente.txt","r");
    if(doc == NULL){
        printf("Nu s-a putut deschide fisierul 'documente.txt'!");
        return 1;
    }
    
    //----------------------------------------------------------------  
    
    // citire fisiere / path fisiere din "documente.txt" folosind alocare dinamica
    char **fisiere = (char**)malloc(1*sizeof(char*));
    fisiere[0] = (char*)malloc(100);
    int nrFisiere=0, i=0;
    while(fgets(fisiere[i],100,doc) != NULL){
        fisiere = (char**)realloc(fisiere,i+1);
        fisiere[i+1] = (char*)malloc(100);
        nrFisiere++;
        i++;
        
        // eliminam caracterul end line
        if(fisiere[nrFisiere-1][strlen(fisiere[nrFisiere-1])-1] == '\n'){
            fisiere[nrFisiere-1][strlen(fisiere[nrFisiere-1])-1] = '\0';
        }
    }

    //----------------------------------------------------------------  
    
    // cautare termen folosind divide et impera
    int s,ok=0;
    for(int i=0; i <nrFisiere; i++){
        s = 0;
        
        char **termeni = NULL; 
        int lungimeTermeni = 0;
        int j = 0;
        
        // deschidem fiecare fisier stocat in 'fisiere'
        FILE *search = fopen(fisiere[i], "r");
        if (search == NULL) {
            printf("Nu s-a putut deschide fisierul %s\n", fisiere[i]);
            break;
        }
        
        // Alocam memorie pentru numarul de linii
        termeni = (char**)malloc(sizeof(char*));
        if (termeni == NULL) {
            printf("Eroare la alocare memorie\n");
            fclose(search);
            break;
        }
        
        // Alocam memorie pentru numarul de caractere de pe prima linie
        termeni[0] = (char*)malloc(30);
        if (termeni[0] == NULL) {
            printf("Eroare la alocare memorie\n");
            fclose(search);
            free(termeni);
            break;
        }
        
        // Incepem citirea
        while (fgets(termeni[j], 30, search) != NULL) {
            // Dam resize la "termeni"
            termeni = (char**)realloc(termeni, (j + 2) * sizeof(char*));
            if (termeni == NULL) {
                printf("Eroare la realocare memorie\n");
                fclose(search);
                free(termeni[j]);
                free(termeni);
                break;
            }
            
            // Alocare
            termeni[j + 1] = (char*)malloc(30);
            if (termeni[j + 1] == NULL) {
                printf("Eroare la alocare memorie\n");
                fclose(search);
                free(termeni[j]);
                free(termeni);
                break;
            }
            
            if(termeni[lungimeTermeni][strlen(termeni[lungimeTermeni])-1] == '\n'){
                termeni[lungimeTermeni][strlen(termeni[lungimeTermeni])-1] = '\0';
            } // eliminam end line
            lungimeTermeni++;
            j++;
        }
        
        // aplicam cautarea divide et impera
        if(searchTerm(termeni, s, lungimeTermeni-1, find) == 1){
            fprintf(index,"%s\n",fisiere[i]);
            ok = 1; // cazul in care termenul nu a fost gasit in niciun fisier
        }
        
        //eliberam memoria
        free(termeni[j]);
        free(termeni);
        fclose(search);
    }
    
    //----------------------------------------------------------------  
    
    //eliberam memoria
    while(nrFisiere){
        free(fisiere[nrFisiere]);
        fisiere[nrFisiere] = NULL;
        nrFisiere--;
    }
    
    //inchidem fisierele
    fclose(doc);
    fclose(input);
    
    // cazul in care termenul nu a fost gasit in niciun fisier
    if(ok == 0)
        fprintf(index,"Termenul nu exista!\n");
    
    // stop clock
    end = clock();
    time_sec = (double)(end-start)/CLOCKS_PER_SEC;
    fprintf(index,"\nTimp: %lf",time_sec);
    
    return 0;
}