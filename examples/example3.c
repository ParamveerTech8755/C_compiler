int main(){
    int x = 1;
    {
        int x = 0;
        x += 98;
        return x;
    }
    return x+2;
}
