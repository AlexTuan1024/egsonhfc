#include<stdio.h>
#include<string.h>

enum response_type {
	DUMP, SECOND_CHANCE, MARRIAGE
};
typedef struct {
	char *name;
	enum response_type type;
} response;

void dump(response r) {
	printf("Dear %s\n", r.name);
	puts("Unfortunately your last date contact us to");
	puts("say that they will not seeing you again.");
}

void second_chance(response r) {
	printf("Dear %s\n", r.name);
	puts("Good news:your last date has asked us to");
	puts("arrange another meeting.Please call ASAP.");
}

void marriage(response r) {
	printf("Dear %s\n", r.name);
	puts("Congratulations!Your last date has contacted");
	puts("us with a proposal of marriage.");
}

void (*replies[])(response) = {dump,second_chance,marriage};

int main(){
    response rs[]={
        {"Mike",DUMP},
        {"Peter",SECOND_CHANCE},
        {"Tony",SECOND_CHANCE},
        {"Steve",MARRIAGE}
    };
    int i=0;
    for(i=0;i<sizeof(rs)/sizeof(response);i++){
        replies[rs[i].type](rs[i]);
    }
    return 0;
}