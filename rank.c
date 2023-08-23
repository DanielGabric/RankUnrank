#include <stdio.h>
typedef long long int LONG;

/////////////////
// Returns k^n //
/////////////////
LONG power(int k, int n){
    int prod=1,i;
    for(i=1; i<=n; ++i) prod *= k;
    return prod;
}
//////////////////////////////////////////////////////////////////////////////////////
// Compute B_k(u,n) given the unbordered prefix indicator and border indicator of u //
//////////////////////////////////////////////////////////////////////////////////////
LONG B(int a[], int b[], int n, int p, int k){
    LONG saveB[n+1];
    int i,j;

    if(n <= 2*p){
        LONG total = 0;
        for(i=1; i<=n-p; ++i) total += a[i]*power(k,n-p-i);
        for(i=n-p+1; i<=n/2; ++i) total += a[i]*b[i-(n-p)];
        return total;
    }else{
        for(j=p; j<=2*p; ++j) saveB[j] = B(a,b,j,p,k);
        for(j=2*p+1; j<=n; ++j){
            saveB[j] = 0;
            for(i=1; i<=p; ++i) saveB[j] += a[i]*power(k,j-p-i);
            for(i=p+1; i<=j/2; ++i) saveB[j] += (power(k,i-p)-saveB[i])*power(k,j-2*i);
        }
        return saveB[n];
    }

    return 0;
}
/////////////////////////////////////////////////////////////////////////////
// Compute the unbordered prefix indicator and border indicator of w[1..n] //
/////////////////////////////////////////////////////////////////////////////
void populateBorderArrays(int w[], int n, int a[], int b[]){
    int PBA[n+1],i,len;

    for(i=1; i<=n; ++i)PBA[i] = a[i] = b[i] = 0;
    
    //Computing the unbordered prefix indicator of w[1..n]
    a[1] = 1;
    i = 2;
    len = 0;
    while(i <= n){
        if(w[i] == w[len+1]) PBA[i++] = (++len);
        else{
            if(len != 0) len = PBA[len];
            else PBA[i++] = 0;
        }   
        a[i-1] = (PBA[i-1] == 0);
    }
    
    //Computing the border indicator of w[1..n]
    i = n;
    while(PBA[i] > 0){
        b[PBA[i]] = 1;
        i = PBA[i];
    }
}
/////////////////////////////////////////////////////
// Return the rank of w[1..n] among bordered words //
/////////////////////////////////////////////////////
LONG rankB(int w[], int n, int k){
    LONG result=0;
    int a[n+1],b[n+1],i,c;

    for(i=1; i<=n; ++i){
        int save = w[i];
        for(c=1; c<=save-1;++c){
            w[i] = c;
            populateBorderArrays(w,i,a,b);
            result += B(a,b,n,i,k);
        }
        w[i] = save;
    }
    
    return result+1;
}
///////////////////////////////////////////////////////
// Return the rank of w[1..n] among unbordered words //
///////////////////////////////////////////////////////
LONG rankU(int w[], int n, int k){
    LONG result=0;
    int i;

    for(i=1; i<=n; ++i)result += (w[i]-1)*power(k,n-i);

    return 2+result-rankB(w,n,k);
}
//////////////////////////////////////////////////////////////////
// Compute the bordered or unbordered word at a particular rank //
//////////////////////////////////////////////////////////////////
void unrank(LONG rank, int n, int k, int w[], int isB){
    int i,j,left,right,mid,save;

    for(i = 1;i <= n;++i)w[i] = 1;

    for(i = 1;i <= n;++i){
        left = 1;
        right = k;
        while(left < right){
            save = w[i];
            mid = (left+right+1)/2;
            w[i] = mid;
            LONG currRank = 0;
            if(isB) currRank = rankB(w,n,k);
            else currRank = rankU(w,n,k);
            if(currRank <= rank) left = mid;
            else{
                w[i] = save;
                right = mid;
            }
        }
    }
}

int main(){
    int n,k,option,w[1000]={0},i;
    LONG rank;

    printf("1. Rank bordered words \n");
    printf("2. Rank unbordered words \n");
    printf("3. Unrank bordered words \n");
    printf("4. Unrank unbordered words \n");

    printf("Enter option number: ");
    scanf("%d", &option);
    printf("Enter the length of the word: ");
    scanf("%d", &n);
    printf("Enter the size of the alphabet: ");
    scanf("%d", &k);

    if(option == 1 || option == 2){
        printf("Enter a word w[1..n] over the alphabet {1,...,%d}\n",k);
        for(i=1; i<=n; ++i){
            printf("Enter w[%d]: ", i);
            scanf("%d", &w[i]);
        }
        printf("Rank: ");
        if(option == 1) printf("%lld\n", rankB(w, n, k));
        else printf("%lld\n", rankU(w, n, k));
    }else if(option == 3 || option == 4){
        printf("Enter rank: ");
        scanf("%lld", &rank);
        unrank(rank, n, k, w, (option == 3));
        for(i=1; i<=n; ++i) printf("%d", w[i]);
        printf("\n");
    }
    return 0;
}


