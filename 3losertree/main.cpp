#include<iostream>
#include<vector>
#include<map>

using namespace std;
class loserTree {
public:
    loserTree (int* thePlayers, int n);     //create the loser tree
    ~loserTree () { delete[] tree; delete[] temp; } //release resources

    void replay (int thePlayer, int value); //replay the game
    void output () const;
    int* theTree () { return tree; }
private:
    int* tree;     //place the loser in the game,but tree[0] is final winner
    int* player;   //the player to compete
    int* temp;    //place the winner to compete
    int numOfPlayer;  //the number of the player to compete

    int lowExt;  //2*(n-s),represent the number of the bottom,s=2^(log2(n-1))
                 //s represent the index of the far left of the bottom
    int offset;   //2*s-1

    int winner (int x, int y) { return player[x] <= player[y] ? x : y; }   //return the winner
    int loser (int x, int y) { return player[x] > player[y] ? x : y; }     //return the loser
    void play (int gamepoint, int leftPlayer, int rightPlayer);  //to comptete,if left<=right left win,else left lose
};
loserTree::loserTree (int* thePlayers, int n) {
    tree = nullptr, temp = nullptr;
    if (n < 2) { return; }
    player = thePlayers;
    numOfPlayer = n;
    tree = new int[n];
    temp = new int[n];

    int i, s;
    for (s = 1; 2 * s <= n - 1; s *= 2);
    lowExt = 2 * (n - s);     //when player begins from index 1,  lowExt=2*(n-s)   offset=2*s-1;
    offset = 2 * s - 1;

    for (i = 2; i <= lowExt; i += 2) {// players of the last layer compete
        play ((i + offset) / 2, i - 1, i);
    }

    //deal with the special point
    if (n % 2 == 1) {  //the left player of game point is from tree[i]
        play (n / 2, temp[n - 1], lowExt + 1);
        i = lowExt + 3;
    } else {
        i = lowExt + 2;
    }

    for (; i <= n; i += 2) { //players of the last but one layer compete
        play ((i - lowExt + n - 1) / 2, i - 1, i);
    }
    tree[0] = temp[1];    //record the winner int the lose tree
}
void loserTree::play (int p, int left, int right) {
    tree[p] = loser (left, right);   //when p is even,the competition only one time 
    temp[p] = winner (left, right);  //why?p is even that means compete only occur int the last layer
                                  //ioslate that special point with lower point
   //this means the players all exists int the lose tree,continue upper until to the top
    while (p % 2 == 1 && p > 1)  //the index of the right player is always odd
    {
        tree[p / 2] = loser (temp[p - 1], temp[p]);   // loser
        temp[p / 2] = winner (temp[p - 1], temp[p]);   // winner

        p /= 2;    //continue unitl reach the root
    }
}
void loserTree::replay (int thePlayer, int newValue) {
    int n = numOfPlayer;
    if (thePlayer <= 0 || thePlayer > n) {
        cout << "the parameter is error" << endl;
        return;
    }
    player[thePlayer] = newValue;
    //get the left player and the right player
    int matchPoint, left, right;      //game point,left player,right player
    if (thePlayer <= lowExt) {       //the player is on the bottom 
        matchPoint = (thePlayer + offset) / 2;
        left = 2 * matchPoint - offset;    // unify to the left player
        right = left + 1;
    } else {   //the player is on the last but one
        matchPoint = (thePlayer - lowExt + n - 1) / 2;

        if (2 * matchPoint == n - 1) {  //special gamepoint
            left = temp[2 * matchPoint];
            right = thePlayer;
        } else {
            left = 2 * matchPoint - (n - 1) + lowExt;
            right = left + 1;
        }
    }

    //to compete
    if (thePlayer == tree[0]) {//the player to replay is the previous winner!!!,ok,that's simple
        for (; matchPoint >= 1; matchPoint /= 2) {// and we note the left and right is not necessary,so the upper code can be simpler
            int loserTemp = loser (tree[matchPoint], thePlayer);
            temp[matchPoint] = winner (tree[matchPoint], thePlayer);
            tree[matchPoint] = loserTemp;
            thePlayer = temp[matchPoint];
        }
    } else {  //it's normal,but the competiton is a bit complex
        tree[matchPoint] = loser (left, right);  //first game
        temp[matchPoint] = winner (left, right);
        if (matchPoint == n - 1 && n % 2 == 1) {   //second game,and it's special
            matchPoint /= 2;
            tree[matchPoint] = loser (temp[n - 1], lowExt + 1);  //fisrt game
            temp[matchPoint] = winner (temp[n - 1], lowExt + 1);
        }
        matchPoint /= 2;  //then all is back to normal,continue competing
        for (; matchPoint >= 1; matchPoint /= 2) {
            tree[matchPoint] = loser (temp[2 * matchPoint], temp[2 * matchPoint + 1]);
            temp[matchPoint] = winner (temp[2 * matchPoint], temp[2 * matchPoint + 1]);
        }
    }
    tree[0] = temp[1];  //place the new winner
}
void loserTree::output () const {
    for (int i = 0; i < numOfPlayer; i++)   //< not <=
    {
        cout << player[tree[i]] << " ";
    }
}


#pragma warning(disable:4996)
int main () { //int players[]={0,10,9,20,6,16,12,90,17};
    freopen ("in.txt", "r", stdin);
    int n, m;
    cin >> n >> m;
    int* players = new int[n + 1];  //the number of valid element is n
    for (int i = 1; i <= n; i++) {
        cin >> players[i];
    }
    loserTree ltree (players, n);
    vector<pair<int, int>> vPair;
    int index, value;
    for (int i = 1; i <= m; i++) {//record the input
        cin >> index >> value;
        pair<int, int>ptemp (index + 1, value);
        vPair.push_back (ptemp);
    }

    ltree.output ();//output the original result
    cout << endl;
    for (int i = 0; i < vPair.size (); i++) {//output the results
        ltree.replay (vPair[i].first, vPair[i].second);
        ltree.output ();
        cout << endl;
    }
    delete[] players;
}