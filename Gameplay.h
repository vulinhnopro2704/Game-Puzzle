#pragma once

#include "CommonFunc.h"
#include "LTexture.h"
#include "LTimer.h"
//#include "LButton.h"

using namespace std;

class Gameplay {
private:
    //Đích 
    vector<vector<int>> Goal;
    //Mảng Chỉ số Puzzle
    vector<vector<int>> a;
    vector<int> b;
    //Tọa độ Ảnh
    vector<pair<int, int>> posIMG;
    //Hàng đợi ưu tiên xử lí Autorun
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> OPEN;
    vector<pair<vector<vector<int>>, int>> FRINGE;
    vector<vector<vector<int>>> CLOSE;
    vector<int> KQ;
    vector<char> ANS;
    vector<int> FATHER;
    vector<pair<int, int >> p;
    int Height;
    bool isRunning;
    int cnt, check, res, checksolve, step, index;
    pair<int, int> Poszero;
    bool checkmove = true;
    int farfromgoal = 0;
    //Tên người chơi
    string PlayerName;
    //Thoi gian
    string timeplay;
    //Đã từng nhấn AutoRun
    bool UsedtoPressAutoRun = false;
    bool ContinueGame = false;
public:
    // Constructor
    Gameplay() : Goal(vector<vector<int>>(6 + 2, vector<int>(6 + 2))),
        a(vector<vector<int>>(6 + 2, vector<int>(6 + 2))),
        b(vector<int>(6 + 2)),
        posIMG(vector<pair<int, int>>(6 * 6 + 1)),
        FRINGE(100000, pair<vector<vector<int>>,
        int>(vector<vector<int>>(6 + 2, vector<int>(6 + 2)), 0)),
        FATHER(vector<int>(100000))
        {
            cnt = 0;
            check = 0;
            checksolve = 0;
            res = 1;
            step = 0;
            isRunning = true;
        }
    //Flag kiểm tra xem có tiếp tục game hay tạo mới
    Gameplay(string PlayerName, bool flag = false) : PlayerName(PlayerName), ContinueGame(flag),  Goal(vector<vector<int>>(6 + 2, vector<int>(6 + 2))),
        a(vector<vector<int>>(6 + 2, vector<int>(6 + 2))),
        b(vector<int>(6 + 2)),
        posIMG(vector<pair<int, int>>(6 * 6 + 1)),
        FRINGE(100000, pair<vector<vector<int>>,
            int>(vector<vector<int>>(6 + 2, vector<int>(6 + 2)), 0)),
        FATHER(vector<int>(100000))
        {
            cnt = 0;
            check = 0;
            checksolve = 0;
            res = 1;
            step = 0;
            isRunning = true;
        }
    void PressReload();
    void SetNguoc(int height);
    void SetUpGame(int Height);
    void CheckRand();
    void Random(int height);
    void display(vector<vector<int>> a);
    void Solve(pair<int, int> p);
    pair<int, int> Pos(int x, vector<vector<int>> v);
    bool CheckTrung(vector<vector<int>> a);
    bool CheckGoal(vector<vector<int>> a);
    int Heuristic(vector<vector<int>> a);
    void KhoiTao();
    void AuToRun(bool &CheckQuit);
    void Clear();
    void setA();
    void setGoal();
    vector<vector<int>> getA();
    vector<vector<int>> getGoal();
    pair<int, int> getPosImg(int i);
    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void Play();
    void update();
    void render();
    void clean();
    bool running() {
        return isRunning;
    };
    void SolveGame();
    int getCnt() {
        return cnt;
    }
    int checkPos(pair<int, int> p);
    void Run();
    static SDL_Event event;
    bool LoadMedia();
    void HandleAuto();
    void SolveMouse(pair<int, int> p);
    void Infile();
    void Outfile();
    bool AddScoretoFile();
};
