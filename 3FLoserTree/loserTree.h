#include<iostream>
#include<vector>
#include<map>
#pragma warning(disable:4996)

using namespace std;
template<class T>
class loserTree
{
    public:
        loserTree(){tree=NULL;};
        loserTree(T* thePlayers,int n){initialize(thePlayers, n);}            //创建输者树
        ~loserTree(){delete [] tree;delete [] temp;} //释放资源

        void initialize(T*, int);
        void replay(int thePlayer); 
        void output() const;
        int* theTree(){return tree;}                //返回输者树
        int winner(){return temp[1];}

   private:
       int* tree;       //内部节点，比赛的输者，tree[0]存放的是最终比赛的赢者
       int* temp;       //存放比赛的赢者
       T* player;     //所要比赛的选手(外部节点)
       
       int numOfPlayer; //选手的个数
 
       int lowExt;      //2*(n-s),最底层外部节点的个数,s=2^(log2(n-1))
                        //s为最底层最左端的内部节点的位置
       int offset;      //2*s-1
 
       int winner(int x,int y){return player[x]<=player[y]?x:y;}    //返回赢者
       int loser(int x,int y){return player[x]<=player[y]?y:x;}      //返回输者
       void play(int gamepoint,int leftPlayer,int rightPlayer);     //比赛,if left<=right left win,else left lose
};
template<class T>
void loserTree<T>::initialize(T* thePlayers,int n)
{
        tree=nullptr,temp=nullptr;

        if(n<2){return;}
        player = thePlayers;
        numOfPlayer = n;

        tree=new int[n];
        temp=new int[n];

        int i,s;

    //计算s, lowExt, offset
        for(s=1;2*s<=n-1;s*=2);
        lowExt=2*(n-s);     
        offset=2*s-1;

    //对所有底层外部节点进行play()
    for(i=2;i<=lowExt;i+=2)
            play((i+offset)/2,i-1,i);
        
    //n为奇数时，会出现一个内节点其孩子分别是内节点和外节点
    //先对这个内节点和外节点进行play() 
    if(n%2==1){
            play(n/2,temp[n-1],lowExt+1);
            i=lowExt+3;
        }else{
            i=lowExt+2;
        }

    //再对剩余的外部节点play()
    for(;i<=n;i+=2)
        play((i-lowExt+n-1)/2,i-1,i);
    
    //将最终的赢者记录在tree[0]
    tree[0]=temp[1];    
}
template<class T>
void loserTree<T>::play(int p,int left,int right)
{
    //根据player[l],player[r]的值，设置tree[p]的值，即在左右子节点l，r之间举办比赛
    //之后如果p是右孩子节点，则继续举行上一层的比赛

    tree[p]=loser(left,right);   
    temp[p]=winner(left,right);  

    while (p%2 == 1 && p > 1)  //内节点p是右孩子且非根
	{
           tree[p/2]=loser(temp[p-1],temp[p]);   
           temp[p/2]=winner(temp[p-1],temp[p]);  
 
           p /= 2;
	}
}
template<class T>
void loserTree<T>::replay(int thePlayer)
{
    int n = numOfPlayer;
    if(thePlayer<=0 || thePlayer>n){
        cout<<"player index is illgal"<<endl;
        return;
    }
    
    //get the left player and the right player
    int matchPoint;     //左右选手的父亲节点
    int left,right;     //左右选手

    if(thePlayer <= lowExt){       //the player is on the bottom 
        matchPoint = (thePlayer+offset)/2; 
        left = 2*matchPoint-offset;    // unify to the left player
        right = left+1;
    }
    else{   //the player is on the last but one
        matchPoint = (thePlayer-lowExt+n-1)/2;
        //theplayer的左兄弟是最后一个内部节点
        if(2*matchPoint == n-1){  
            left = temp[2*matchPoint];
            right = thePlayer;
        }else{
            left = 2*matchPoint-(n-1)+lowExt;
            right = left+1;
        }
    }

    //重新比赛
    //重新比赛的选手在之前胜者的位置
    if(thePlayer==tree[0]){//the player to replay is the previous winner!!!,ok,that's simple
        for(;matchPoint>=1;matchPoint/=2)
        {   //上次比赛的输者已经记录在tree[]中 
            int loserTemp=loser(tree[matchPoint],thePlayer);
            temp[matchPoint]=winner(tree[matchPoint],thePlayer);
            tree[matchPoint]=loserTemp;
            thePlayer=temp[matchPoint];
        }
    }
    else{  
        //否则从该节点到根的路径需重新比赛
        tree[matchPoint]=loser(left,right);  //first game
        temp[matchPoint]=winner(left,right);
        if(matchPoint==n-1&&n%2==1){   //second game,and it's special
            matchPoint/=2;
            tree[matchPoint]=loser(temp[n-1],lowExt+1);  //fisrt game
            temp[matchPoint]=winner(temp[n-1],lowExt+1);
        }
        matchPoint/=2;  //then all is back to normal,continue competing
        for(;matchPoint>=1;matchPoint/=2)
        {
            tree[matchPoint]=loser(temp[2*matchPoint],temp[2*matchPoint+1]);
            temp[matchPoint]=winner(temp[2*matchPoint],temp[2*matchPoint+1]);
        }
    }
    tree[0]=temp[1];  //place the new winner
}
template<class T>
void loserTree<T>::output() const
{
     for(int i=0;i<numOfPlayer;i++)   //< not <=
     {
        cout<<player[tree[i]]<<endl;}
}

/*
8 1
10 9 20 6 16 12 90 17
3 15
*/
/*
int main()
{ //int players[]={0,10,9,20,6,16,12,90,17};
    int n,m;
    cin>>n>>m;
    int* players=new int[n+1];  //the number of valid element is n
    for(int i=1;i<=n;i++)
    {
        cin>>players[i];
    }
    loserTree<int> ltree(players,n);
    vector<pair<int,int>> vPair;
    int index,value;
    for(int i=1;i<=m;i++)
    {//record the input
        cin>>index>>value;
        pair<int,int>ptemp(index+1,value);
        vPair.push_back(ptemp);
    }

    ltree.output();//output the original result
    cout<<endl;
    for(int i=0;i<vPair.size();i++)
    {//output the results
        players[vPair[i].first] = vPair[i].second;
        ltree.replay(vPair[i].first);
        ltree.output();
        cout<<endl;
      }
   delete [] players;
   system("pause");
}
*/