vector<vector<int> > GameOfLife::SimulateLife(vector<vector<int> > &board, int life_cycles){
    vector<vector<int> > present = board;
    vector<vector<int> > final = board;
    thread myThreads[4];
    
    pthread_barrier_init(&mybarrier,NULL,4);

    for(size_t i = 0; i < 4; i++){
        GameOfLife x;
        myThreads[i] = thread(&GameOfLife::nextGrid, &x, i, life_cycles, ref(board), ref(final));
    }
    //here we join all 4 threads
    for(auto &t : myThreads){
        t.join();
    }
    
    return final;
}

//nextGrid is used to give each thread the work to do and then call them to join
void GameOfLife::nextGrid(int i, int life_cycles, vector<vector<int> > &board, vector<vector<int> > &final){
    mymutex.lock();
    vector<vector<int> > futureGrid = board;
    vector<vector<int> > presentGrid = board;
    mymutex.unlock();
    pthread_barrier_wait(&mybarrier);
    for(int j = 0; j < life_cycles; j++){
        Conway(presentGrid, futureGrid, i);
        pthread_barrier_wait(&mybarrier);
        mymutex.lock();
        if(i == 0){
            presentGrid = futureGrid;
        }
        mymutex.unlock();
        pthread_barrier_wait(&mybarrier);
    }
    pthread_barrier_wait(&mybarrier);
    mymutex.lock();
    if(i == 0){
        final = futureGrid;
    }
    mymutex.unlock();
    pthread_barrier_wait(&mybarrier);
}