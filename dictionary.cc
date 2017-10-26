#include <node.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#define HASHTABLE_SIZE 300
#define LENGTH 45
#define DICTIONARY "large"


FILE *log = fopen("new.txt","a");


typedef struct node_word
{
    char word[LENGTH+1];
    struct node_word* next;
}node_word;

node_word* hashtable[HASHTABLE_SIZE];

bool loaded=false;

int word_count=0;

int hash_it(char* needs_hashing);

bool load(const char* dictionary)
{
    for(int i=0;i<HASHTABLE_SIZE;i++)
    {
        hashtable[i]=NULL;
    }
    
    
    
    
    FILE* fp=fopen(dictionary,"r");
    if(fp==NULL)
    {
        fprintf(log,"couldn't open dictionary.\n");
        return false;
    }
    
    while(true)
    {
        node_word* new_node=(node_word*)malloc(sizeof(node_word));
        if(new_node==NULL)
        {
            fprintf(log,"Memory allocation failed.\n");
            return 1;
        }
        fscanf(fp,"%s",new_node->word);
        new_node->next=NULL;
        
        if(feof(fp))
        {
            free(new_node);
            loaded=true;
            return true;
        }
        
        int h=hash_it(new_node->word);
        node_word* head=hashtable[h];
        
        word_count++;
        
        if(head==NULL)
        {
            hashtable[h]=new_node;
        }
        else
        {
            new_node->next=hashtable[h];
            hashtable[h]=new_node;
            
        }
        
    }
    
}




int hash_it(char* needs_hashing)
{
    unsigned int hash = 0;
    for (int i=0, n=strlen(needs_hashing); i<n; i++)
        hash = (hash << 2) ^ needs_hashing[i];
    return hash % HASHTABLE_SIZE;
}





bool check(const char* word)
{
    char word_copy[strlen(word)+1];
    unsigned int i=0;
    for(;i<strlen(word);i++)
        word_copy[i]=tolower(word[i]);
    
    word_copy[i]='\0';
    
    
    int h=hash_it(word_copy);
    
    node_word* new_node=hashtable[h];
    
    
    
    while(new_node!=NULL)
    {
        if(strcmp(word_copy,new_node->word)==0)
            return true;
        
        
        else
            new_node=new_node->next;
    }
    
    return false;
}

int size()
{
    if(loaded)
        return word_count;
    
    else return 0;
}

bool unload()
{
    
    for(int i=0;i<HASHTABLE_SIZE;i++)
    {
        node_word* cursor=hashtable[i];
        
        while(cursor!=NULL)
        {
            node_word* temp=cursor;
            cursor=cursor->next;
            free(temp);
        }
    }
    
    loaded=false;
    return true;
}

void dictionary_check(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = args.GetIsolate();
    
    
    char const  *dictionary = DICTIONARY;
    
    bool loaded = load(dictionary);
    
    if (!loaded)
    {
        auto message = v8::String::NewFromUtf8(isolate, "Could not load");
        args.GetReturnValue().Set(message);
        return;
    }
    
    v8::String::Utf8Value val(args[0]->ToString());
    
    std::string text1 (*val);
    text1=text1+" ";
    //Adding a space at the end so the last word is also checked.
    //In the earlier version there was an EOF generated so this was not needed.
    char * text = new char [text1.length()+1];
    std::strcpy (text, text1.c_str());
    
    
    int index = 0,indi=0, misspellings = 0, words = 0;
    char word[LENGTH+1];
    int i=0;
    
    while(text[i++]!='\0');
   
    for (int c = text[indi]; text[indi]!='\0'; c = text[++indi])
    {
        
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
          
            // append character to word
            word[index] = c;
            index++;
            
            // ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // consume remainder of alphabetical string
                while ( (c=text[indi++]) != '\0' && isalpha(c));
                
                // prepare for new word
                index = 0;
            }
            word[index]='\0';
           // printf("%s\n",word);
        }
        
        // ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // consume remainder of alphanumeric string
            while ((c=text[indi++]) != '\0' && isalpha(c));
            
            // prepare for new word
            index = 0;
        }
        
        // we must have found a whole word
        // DV note: because we hit a space or punctuation which signifies EOW
        else if (index > 0)
        {
            fprintf(log,"%s\n",word);
           // printf("Comparing word\n");
            
            // terminate current word
            word[index] = '\0';
           // printf("%s\n",word);
            
            // update counter
            words++;
            

            bool misspelled = !check(word);
            
            if (misspelled)
            {
                //printf("%s is misspelled\n",word);
                misspellings++;
            }
            
            index = 0;
        }
    }
    char temp_answer[2];
    sprintf(temp_answer,"%d",misspellings);
    const char *answer=temp_answer;


    
    bool unloaded = unload();
    
    if (!unloaded)
    {
        auto message = v8::String::NewFromUtf8(isolate, "Could not unload");
        args.GetReturnValue().Set(message);
        return;
    }
    
    
    auto message = v8::String::NewFromUtf8(isolate, answer);
    args.GetReturnValue().Set(message);
    
    fclose(log);
}

void dictionary_add(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = args.GetIsolate();
    v8::String::Utf8Value val(args[0]->ToString());
    
    std::string word (*val);
    char *pointer = new char [word.length()];
    std::strcpy (pointer, word.c_str());
    
    char return_value[50];
    char *return_final=return_value;
    
    std::ofstream ofs;
    ofs.open("large", std::ofstream::out | std::ofstream::app);
   // FILE* fp=fopen("large","a");
   /* if(fp==NULL)
    {
        auto message = v8::String::NewFromUtf8(isolate,"Unable to open Dictionary");
        args.GetReturnValue().Set(message);
        return;
    }
    */
    ofs<<word<<std::endl;
    ofs.close();
    //fprintf(fp,"%s",pointer);
    //fclose(fp);
    sprintf(return_value,"%s Added Successfully",pointer);
    auto message = v8::String::NewFromUtf8(isolate,return_final);
    args.GetReturnValue().Set(message);
    return;
    
}


void Initialize(v8::Local<v8::Object> exports) {
    NODE_SET_METHOD(exports, "dictionary_check", dictionary_check);
    NODE_SET_METHOD(exports, "dictionary_add", dictionary_add);

}

NODE_MODULE(module_name, Initialize)


