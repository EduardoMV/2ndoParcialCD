#ifndef BLACKJACK_H
#define BLACKJACK_H

struct card {
    char symb; 
    char numb;
    int active;//1 if true 0 if false;
};

void init_deck(struct card *empty_deck){

    int n = 52;

    char symbols[4] = { 'C', 'H', 'D','S'};
    char numbers[13] = {'2','3','4','5','6','7','8','9','X','J','Q','k','A'};

    for(int c = 0; c < n; c++){
        int numb = c % 13;
        int symb = (c / 13) % 4;

        struct card new_card = { symbols[symb], numbers[numb], 0 };
        empty_deck[c] = new_card;
    }
    return;
}


void shuffle_deck(struct card *deck){
    int n = 52;
    srand(time(NULL));
    for(int c = 0; c < n; c++){
        int idx = (rand() % n);
        
        struct card temp;
        temp = deck[c];
        deck[c] = deck[idx];
        deck[idx ]= temp;
    }

}

struct card takeCard(struct card * deck){
    struct card res; 
    //logic goes here
    
    return res;
}

#endif /* BLACKJACK_H */