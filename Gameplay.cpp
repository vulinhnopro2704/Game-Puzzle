#include "Gameplay.h"
#include "LButton.h"
#include "Main_Menu.h"
#include "Winner.h"
#include "LeaderBoard.h"

LeaderBoard LB;
LTexture ButtonBack, ButtonReload, ButtonAutoRun, ButtonMode, StepTexture, SolveMode, LoadingImage, FrameLoadingImage;
LButton gButtonBack, gButtonReload, gButtonAutoRun;

SDL_Rect ButtonReloadRect[] = { {0, 0, 142, 142}, {0, 175, 142, 142}, {0,350 , 142, 142} };
SDL_Rect ButtonBackRect[] = { {0, 0, 156, 156}, {0, 180, 156, 156}, {0, 361, 156, 156} };
SDL_Rect ButtonAutoRunRect[] = { {0, 0, 327, 65}, {0, 182, 327, 65}, {0, 356, 327, 65} };
SDL_Rect ButtonModeRect[] = { {0, 0, 186, 58}, {0, 70, 186, 58}, {0, 140, 186, 58}, {0, 210, 186, 58} };
LTimer timer;
LTexture timing;
Mix_Chunk* gSlide = NULL;
Mix_Music* gSoundTrack = NULL;

LTexture Background, GoalImage;

bool isPressBack = false;
bool WinnerScreenOff = false;
bool isPressReload = false;

bool Gameplay::LoadMedia() 
{
    std::string index = std::to_string(Order % TOTAL_IMAGE);
    std::string imagePath = "Data//MenuImage//MenuImage" + index + ".png";
    bool success = true;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    if (!ButtonAutoRun.loadFromFile("IMG//AutoRun.png"))
    {
        success = false;
    }
    else if (!ButtonBack.loadFromFile("IMG//Back.png"))
    {
        success = false;
    }
    else if (!ButtonReload.loadFromFile("IMG//Reload.png"))
    {
        success = false;
    }
    else if (!ButtonMode.loadFromFile("IMG//Mode.png"))
    {
        success = false;
    }
    else if (!Background.loadFromFile("IMG//PlayGame.PNG"))
    {
        success = false;
    }
    else if (!GoalImage.loadFromFile(imagePath))
    {
        success = false;
    }
    else if (!LoadingImage.loadFromFile("IMG//Loading.png"))
    {
        success = false;
    }
    else if (!FrameLoadingImage.loadFromFile("IMG//FrameLoadingImage.png"))
    {
        success = false;
    }
    else if (!(gSoundTrack = Mix_LoadMUS("Music//Rondo Alla.mp3")))
    {
        printf("Failed to load SoundTrack! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    else if (!(gSlide = Mix_LoadWAV("Music//low.wav")))
    {
        printf("Failed to load gSlide sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    else {
        gButtonReload.SetAllValue(985, 357, 142, 142);
        gButtonAutoRun.SetAllValue(896, 523, 327, 65);
        gButtonBack.SetAllValue((SCREEN_WIDTH - 156 / 2), - (156 / 2), 156, 156);
    }
    return success;
}

std::string millisecondsToTimeString(Uint32 milliseconds) {
    Uint32 seconds = milliseconds / 1000; // Chuyển từ mili giây sang giây
    Uint32 minutes = seconds / 60; // Chuyển từ giây sang phút
    seconds %= 60; // Lấy số giây còn lại sau khi tính phút

    // Sử dụng std::to_string để chuyển số thành chuỗi và đảm bảo rằng chúng có hai chữ số
    std::string mm = (minutes < 10 ? "0" : "") + std::to_string(minutes);
    std::string ss = (seconds < 10 ? "0" : "") + std::to_string(seconds);

    return mm + " : " + ss;
}

void Gameplay::CheckRand()
{
    //Kiểm tra điều kiện random
    set<int> s;
    for (int i = n; i >= 1; i--) {
        for (int j = n; j >= 1; j--) {
            if (a[i][j] != 0) {
                s.insert(a[i][j]);
                set<int> ::iterator it = s.find(a[i][j]);
                if (it != s.begin()) {
                    check += distance(s.begin(), it);
                }
            }
        }
    }
}

void Gameplay::Random(int height)
{
    //Hàm random + tính tọa độ ảnh
    Height = height;
    while (1) {
        int tam = 0;
        check = 0;
        b.resize(n * n);
        for (int i = 0; i <= n * n - 1; i++) b[i] = i;
        srand(time(NULL));
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++) {
                int tmp = (rand() + tam++) % b.size();
                a[i][j] = b[tmp];
                //Tính tọa độ ảnh
                posIMG[a[i][j]] = { Height / n * (j - 1) + 89, Height / n * (i - 1) + 85 };
                //Tìm hàng chứa ô trống
                if (a[i][j] == 0) Poszero = { i, j };
                b.erase(b.begin() + tmp);
                //cout << posIMG[a[i][j]].first << " " << posIMG[a[i][j]].second << endl;
            }
        }
        CheckRand();
        if ((check + Poszero.first * (n % 2 + 1)) % 2 == 0) break;
    }
}

void Gameplay::display(vector<vector<int>> a)
{
    //Hàm in ra màn hình
    cnt = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            printf("%3d ", a[i][j]);
            if (a[i][j] != Goal[i][j]) cnt = 0;
        }
        cout << endl;
    }
}

bool isMoved = false;

void Gameplay::Solve(pair<int, int> p)
{

    int W = a[p.first][p.second];
    // Tính 4 vị trí xung quanh ô đang xét
    int x = a[p.first - 1][p.second], y = a[p.first][p.second - 1], z = a[p.first + 1][p.second], t = a[p.first][p.second + 1];
    // Kiểm tra xem ô nào trống thì đổi chỗ và vị trí của ô trống với ô đang xét
    if (!x) {
        swap(a[p.first - 1][p.second], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        Poszero = { p.first, p.second };
        if (!checksolve)
        {
            ++step;
            if (!timer.isStarted())
                timer.start();
        }
        isMoved = true;
        return;
    }
    if (!y) {
        swap(a[p.first][p.second - 1], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        Poszero = { p.first, p.second };
        if (!checksolve)
        {
            ++step;
            if (!timer.isStarted())
                timer.start();
        }
        isMoved = true;
        return;
    }
    if (!z) {
        swap(a[p.first + 1][p.second], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        Poszero = { p.first, p.second };
        if (!checksolve)
        {
            ++step;
            if (!timer.isStarted())
                timer.start();
        }
        isMoved = true;
        return;
    }
    if (!t) {
        swap(a[p.first][p.second + 1], a[p.first][p.second]);
        swap(posIMG[W], posIMG[0]);
        Poszero = { p.first, p.second };
        if (!checksolve)
        {
            ++step;
            if (!timer.isStarted())
                timer.start();
        }
        isMoved = true;
        return;
    }
}

pair<int, int> Gameplay::Pos(int x, vector<vector<int>> v)
{
    //Hàm trả về tọa độ của số x ở trong vector v
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (v[i][j] == x) {
                return { i, j };
            }
        }
    }
}

bool Gameplay::CheckTrung(vector<vector<int>> a)
{
    //Hàm kiểm tra xem vector được sinh ra đã nằm trong CLOSE chưa
    for (int i = 0; i < CLOSE.size(); i++) {
        if (a == CLOSE[i]) return true;
    }
    return false;
}

bool Gameplay::CheckGoal(vector<vector<int>> a)
{
    // Hàm kiểm tra xem trạng thái a có phải là trạng thái đích cần xét đến không
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (a[i][j] != Goal[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

int Gameplay::Heuristic(vector<vector<int>> a)
{
    // Hàm Heuristic
    double d = 0, A = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (a[i][j] != 0) {
                int row = (a[i][j] + n - 1) / n;
                int col = (a[i][j] - 1) % n + 1;
                d += pow(i - row, 2) + pow(j - col, 2);
                //				cout << a[i][j] << " " << pow(i - row, 2) + pow(j - col, 2) << endl;
            }
            if (a[i][j] == Goal[i][j + 1] && a[i][j + 1] == Goal[i][j]) A++;
            if (a[i][j] == Goal[i + 1][j] && a[i + 1][j] == Goal[i][j]) A++;
        }
    }
    double h = round(d - 0.15 * d + A * 2);
    return int(h);
}

void Gameplay::KhoiTao()
{
    cout << "BYE\n";
    // Hàm khởi tạo thêm vào FRINGE các trạng thái hợp lệ xung quanh ô trống
    pair<int, int> p = Pos(0, a);
    FRINGE[0].first = a;
    int g = 1;
    int x = a[p.first - 1][p.second], y = a[p.first][p.second - 1], z = a[p.first + 1][p.second], t = a[p.first][p.second + 1];
    if (x != n * n) {
        vector<vector<int> > tmp = a;
        swap(tmp[p.first][p.second], tmp[p.first - 1][p.second]);
        FRINGE[res].first = tmp;
        FRINGE[res].second = x;
        int h = Heuristic(tmp);
        // Thêm vào hàng đợi ưu tiên OPEN f ( = g + h), vị trí của mảng thêm vào (res), chi phí đi từ lúc đầu đến trạng thái hiện tại(g)
        OPEN.push({ g + h, {g, res} });
        farfromgoal = max(g + h, farfromgoal);
        //Lưu cha của mảng thứ res là 0;
        FATHER[res] = 0;
        res++;
    }
    if (y != n * n) {
        vector<vector<int> > tmp = a;
        swap(tmp[p.first][p.second], tmp[p.first][p.second - 1]);
        FRINGE[res].first = tmp;
        FRINGE[res].second = y;
        int h = Heuristic(tmp);
        OPEN.push({ g + h, {g, res} });
        farfromgoal = max(g + h, farfromgoal);
        FATHER[res] = 0;
        res++;
    }
    if (z != n * n) {
        vector<vector<int> > tmp = a;
        swap(tmp[p.first][p.second], tmp[p.first + 1][p.second]);
        FRINGE[res].first = tmp;
        FRINGE[res].second = z;
        int h = Heuristic(tmp);
        OPEN.push({ g + h, {g, res} });
        farfromgoal = max(g + h, farfromgoal);
        FATHER[res] = 0;
        res++;
    }
    if (t != n * n) {
        vector<vector<int> > tmp = a;
        swap(tmp[p.first][p.second], tmp[p.first][p.second + 1]);
        FRINGE[res].first = tmp;
        FRINGE[res].second = t;
        int h = Heuristic(tmp);
        OPEN.push({ g + h, {g, res} });
        farfromgoal = max(g + h, farfromgoal);
        FATHER[res] = 0;
        res++;
    }
}

void Gameplay::AuToRun(bool &CheckQuit)
{
    int status = 0;
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

    // Đặt màu vẽ là đen (0, 0, 0, 200) - mờ
    SDL_SetRenderDrawColor(gRenderer, 0xDC, 0xBA, 0x79, 255 * 23 / 100);

    // Tạo một SDL_Rect có kích thước như bạn muốn
    SDL_Rect FillRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    // Vẽ hình chữ nhật mờ lên Renderer
    SDL_RenderFillRect(gRenderer, &FillRect);
    // Đặt lại chế độ blend về mặc định
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
    FrameLoadingImage.render(292, 119);
    SDL_Rect LoadingImageRect = { 0, 0, 454, 132 };
    LoadingImage.render((SCREEN_WIDTH - 454) / 2, (SCREEN_HEIGHT - 132) / 2, &LoadingImageRect);
    SDL_RenderPresent(gRenderer);
    cout << "HELLO\n";
    display(a);
    //Khởi tạo trạng thái ban đầu.
    KhoiTao();
    // Curent là trạng thái đang xét hiện t ại
    vector<vector<int>> Curent;
    int k = 1, l;
    bool isQuit = false;
    int present = 1e9;
    SDL_Event e;
    while (!OPEN.empty()) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
            {
                isQuit = true;
                CheckQuit = true;
                outGame = true;
                return;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                isQuit = true;
                CheckQuit = true;
                outGame = true;
                return;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.motion.x >= 543 && e.motion.y >= 468 && e.motion.x <= 543 + 194 && e.motion.y <= 468 + 66)
                {
                    checksolve = 0;
                    CheckQuit = true;
                    //KQ.clear();
                    //CLOSE.clear();
                    //while (!OPEN.empty()) {
                    //    OPEN.pop();
                    //}
                    //res = 1;
                    return;
                }
            } 
        }
        int g = OPEN.top().second.first;
        // lấy ra vị trí của trạng thái có chi phí thấp nhất trong OPEN
        l = OPEN.top().second.second;
        cout << "He: " << OPEN.top().first << endl;
        present = min(present, OPEN.top().first);
        // Cho trạng thái hiện tại đang xét là trạng thái tối ưu (vị trí l đã tính)
        Curent = FRINGE[l].first;
        // Đẩy trạng thái hiện tại vào CLOSE ( xác nhận trạng thái này đã xét)
        CLOSE.push_back(Curent);
        // Xóa đi trạng thái vừa xét ra khỏi OPEN
        OPEN.pop();
        // Nếu trạng thái đang xét là trạng thái đích thì thoát vòng lặp
        if (CheckGoal(Curent)) break;

        pair<int, int> p = Pos(0, Curent);
        int x = Curent[p.first - 1][p.second], y = Curent[p.first][p.second - 1], z = Curent[p.first + 1][p.second], t = Curent[p.first][p.second + 1];
        // Thêm các trạng thái hợp lệ được sinh ra bởi trạng thái đang xét vào OPEN
        if (x != n * n) {
            vector<vector<int> > tmp = Curent;
            swap(tmp[p.first][p.second], tmp[p.first - 1][p.second]);
            // Nếu trạng thái này chưa nằm trong CLOSE thì ta thêm vào OPEN
            if (!CheckTrung(tmp)) {
                int h = Heuristic(tmp);
                // Thêm vào hàng đợi ưu tiền OPEN f ( = g + h), vị trí của mảng thêm vào (res), chi phí đi từ lúc đầu đến trạng thái hiện tại(g)
                OPEN.push({ g + h, {g, res} });
                // Đặt cha của trạng thái đang xét hiện tại là vị trí của trạng thái vừa sinh ra nó (l)
                FATHER[res] = l;
                FRINGE[res].first = tmp;
                FRINGE[res].second = x;
                res++;
            }
        }
        if (y != n * n) {
            vector<vector<int> > tmp = Curent;
            swap(tmp[p.first][p.second], tmp[p.first][p.second - 1]);
            if (!CheckTrung(tmp)) {
                int h = Heuristic(tmp);
                OPEN.push({ g + h, {g, res} });
                FATHER[res] = l;
                FRINGE[res].first = tmp;
                FRINGE[res].second = y;
                res++;
            }
        }
        if (z != n * n) {
            vector<vector<int> > tmp = Curent;
            swap(tmp[p.first][p.second], tmp[p.first + 1][p.second]);
            if (!CheckTrung(tmp)) {
                int h = Heuristic(tmp);
                OPEN.push({ g + h, {g, res} });
                FATHER[res] = l;
                FRINGE[res].first = tmp;
                FRINGE[res].second = z;
                res++;
            }
        }
        if (t != n * n) {
            vector<vector<int> > tmp = Curent;
            swap(tmp[p.first][p.second], tmp[p.first][p.second + 1]);
            if (!CheckTrung(tmp)) {
                int h = Heuristic(tmp);
                OPEN.push({ g + h, {g, res} });
                FATHER[res] = l;
                FRINGE[res].first = tmp;
                FRINGE[res].second = t;
                res++;
            }
        }
        k++;
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
        FrameLoadingImage.render(292, 119);
        status = 9 - (-1 + sqrt(1 + present * 288 / farfromgoal))/2;
        //cout << present << " " << (-1 + sqrt(1 + present * 288 / farfromgoal)) / 2 << " " << status << endl;
        LoadingImageRect = { 0, status * 140, 454, 132 };
        LoadingImage.render((SCREEN_WIDTH - 454) / 2, (SCREEN_HEIGHT - 132) / 2, &LoadingImageRect);
        SDL_RenderPresent(gRenderer);
        //cout << k << endl;
        //display(Curent);
    }
        // Đẩy trạng thái đích vào KQ
        KQ.push_back(l);
        // Tìm các trạng thái đã suy ra được trạng thái đích
        while (FATHER[l] != 0) {
            KQ.push_back(FATHER[l]);
            l = FATHER[l];
        }
        cout << "KQ: " << KQ.size() << endl;
        index = KQ.size();
        FrameLoadingImage.render(292, 119);
        LoadingImageRect = { 0, 9 * 140, 454, 132 };
        LoadingImage.render((SCREEN_WIDTH - 454) / 2, (SCREEN_HEIGHT - 132) / 2, &LoadingImageRect);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(500);
}

void Gameplay::Clear()
{
    // Đặt isRunning thành false để kết thúc vòng lặp chính
    isRunning = false;
    FATHER.clear();
    FRINGE.clear();
    CLOSE.clear();
    KQ.clear();
    while (!OPEN.empty()) {
        OPEN.pop();
    }
    res = 0;
}



void Gameplay::setA()
{
    //Đặt tráng thái ban đầu của vecto a
    for (int i = 0; i <= n + 1; i++) {
        for (int j = 0; j <= n + 1; j++) {
            a[i][j] = n * n;
        }
    }
}

void Gameplay::setGoal()
{
    // Đặt trạng thái đích
    int tmp = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            Goal[i][j] = tmp;
            //cin >> a[i][j];
            tmp++;
        }
    }
    Goal[n][n] = 0;
}

vector<vector<int>> Gameplay::getA()
{
    return a;
}

vector<vector<int>> Gameplay::getGoal()
{
    return Goal;
}

pair<int, int> Gameplay::getPosImg(int i) {
    return posIMG[i];
}

int Gameplay::checkPos(pair<int, int> p)
{
    // Kiểm tra xem ô nào đang chứa tọa độ p ( dùng để xử lí sự kiện )
    for (int i = 1; i <= n * n - 1; i++)
    {
        if (p.first >= posIMG[i].first && p.first <= posIMG[i].first + Height / n
            && p.second >= posIMG[i].second && p.second <= posIMG[i].second + Height / n) return i;
    }
    return 0;
}

void Gameplay::SolveMouse(pair<int, int> p)
{
    if (p == Poszero) return;
    if (p.first == Poszero.first)
    {
        int i = p.first;
        if (p.second < Poszero.second)
        {
            for (int j = Poszero.second - 1; j >= p.second; j--)
            {
                swap(posIMG[a[i][j]], posIMG[0]);
                swap(a[i][j], a[i][j + 1]);
            }
        }
        else
        {
            for (int j = Poszero.second + 1; j <= p.second; j++)
            {
                swap(posIMG[a[i][j]], posIMG[0]);
                swap(a[i][j], a[i][j - 1]);
            }
        }
        Poszero = p;
        if (!checksolve)
        {
            ++step;
            if (!timer.isStarted())
                timer.start();
        }
        isMoved = true;
    }
    else if (p.second == Poszero.second)
    {
        int j = p.second;
        if (p.first < Poszero.first)
        {
            for (int i = Poszero.first - 1; i >= p.first; i--)
            {
                swap(posIMG[a[i][j]], posIMG[0]);
                swap(a[i][j], a[i + 1][j]);
            }
        }
        else
        {
            for (int i = Poszero.first + 1; i <= p.first; i++)
            {
                swap(posIMG[a[i][j]], posIMG[0]);
                swap(a[i][j], a[i - 1][j]);
            }
        }
        Poszero = p;
        if (!checksolve)
        {
            ++step;
            if (!timer.isStarted())
                timer.start();
        }
        isMoved = true;
    }
}

void Gameplay::Infile()
{
    cout << "Infile run" << endl;
    string tenfile = "ContinueGame//puzzle";
    string N = to_string(n);
    tenfile += N + ".txt";
    ifstream inFile(tenfile);
    int status;
    inFile >> status;
    if (status)
    {
        if (inFile.is_open())
        {
            for (int i = 1; i <= n; i++)
            {
                for (int j = 1; j <= n; j++)
                {
                    inFile >> a[i][j];
                }
            }

            for (int i = 0; i < n * n; i++)
            {
                inFile >> posIMG[i].first >> posIMG[i].second;
            }

            inFile >> Poszero.first >> Poszero.second;

            inFile.close();
        }
    }
    display(a);
    cout << endl;
    for (int i = 0; i < n * n; i++)
    {
        cout << posIMG[i].first <<" " << posIMG[i].second << endl;
    }
    cout << Poszero.first << " " << Poszero.second;
}

bool Gameplay::AddScoretoFile()
{
    const string filename = "LeaderBoard-Record//HighScore" + to_string(n) + ".txt";
    std::ofstream file(filename, std::ios::app);

    if (file.is_open()) {
        file << PlayerName << "," << timeplay << "," << step << std::endl;
        file.close();
        std::cout << "Data has been saved to file " << filename << std::endl;
        return true;
    }
    else {
        std::cerr << "Unable to open file to save data." << std::endl;
        return false;
    }
}

void Gameplay::Outfile()
{
    string tenfile = "ContinueGame/puzzle";
    string N = to_string(n);
    tenfile += N + ".txt";
    std::remove(tenfile.c_str()); // Xóa tệp cũ nếu tồn tại
    ofstream outFile(tenfile);
    if (CheckGoal(a))
    {
        outFile << 0 << endl;
    }
    else
    {
        outFile << 1 << endl;
        if (outFile.is_open())
        {
            for (int i = 1; i <= n; i++)
            {
                for (int j = 1; j <= n; j++)
                {
                    outFile << a[i][j] << " ";
                }
                outFile << endl;
            }

            for (int i = 0; i < n * n; i++)
            {
                outFile << posIMG[i].first << " " << posIMG[i].second << endl;
            }

            outFile << Poszero.first << " " << Poszero.second;

            outFile.close();
        }
    }
}

//Gameplay::~Game() {};

GameObject** Number;
GameObject** StartGame;
SDL_Event Gameplay::event;
SDL_Texture* PlayerTex;
SDL_Rect srcR, destR;

std::vector<SDL_Texture*> CutTextureIntoPieces(SDL_Texture* texture, int n) {
    std::vector<SDL_Texture*> textures;

    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

    int pieceWidth = textureWidth / n;
    int pieceHeight = textureHeight / n;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            SDL_Rect srcRect = { j * pieceWidth, i * pieceHeight, pieceWidth, pieceHeight };
            SDL_Texture* pieceTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, pieceWidth, pieceHeight);

            // Thiết lập mục tiêu vẽ cho texture
            SDL_SetRenderTarget(gRenderer, pieceTexture);

            // Xóa texture
            SDL_RenderClear(gRenderer);

            // Sao chép một phần của texture gốc vào texture đích
            SDL_RenderCopy(gRenderer, texture, &srcRect, NULL);

            // Đặt lại mục tiêu vẽ về texture gốc
            SDL_SetRenderTarget(gRenderer, NULL);

            textures.push_back(pieceTexture);
        }
    }

    return textures;
}

void Gameplay::SetNguoc(int height)
{
    Height = height;
    int tmp = n * n;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            a[i][j] = tmp--;
            if (i == 1 && j == 1) a[i][j] = 0;
            //cin >> a[i][j];
            //Tính tọa độ ảnh
            posIMG[a[i][j]] = { Height / n * (j - 1) + 89, Height / n * (i - 1) + 85 };
            //Tìm hàng chứa ô trống
            if (a[i][j] == 0) Poszero = { i, j };
        }
    }
    CheckRand();
    if ((check + Poszero.first * (n % 2 + 1)) % 2 == 0) cout << "Duoc\n";
    else cout << "Khong the giai\n";
}
void Gameplay::SetUpGame(int height)
{
    setA();
    if (ContinueGame)
    {
        Height = height;
        Infile();
        ContinueGame = false;
        Mode = 1;
    }
    else if (Mode == 1)
        Random(height);
    else if (Mode == 2)
        SetNguoc(height);
   /* cout << "Hello\n";
    cout << Poszero.first << " " << Poszero.second << endl;
    display(a);
    for (int i = 0; i < n * n; i++) cout << posIMG[i].first << " " << posIMG[i].second << endl;*/
    setGoal();
    //Load ảnh lớn
    std::string index = std::to_string(Order % TOTAL_IMAGE);
    std::string imagePath = "Data//MenuImage//MenuImage" + index + ".png";

    SDL_Surface* imageSurface = IMG_Load(imagePath.c_str());

    SDL_Texture* largeTexture = SDL_CreateTextureFromSurface(gRenderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    //Tính kích thước của mỗi phần nhỏ
    int smallWidth = 589 / n;
    vector<SDL_Texture*> CutPicture;
    CutPicture = CutTextureIntoPieces(largeTexture, n);
    Number = new GameObject * [n * n + 1];

    // Tạo và lưu các phần nhỏ vào mảng Number
    Number[0] = new GameObject(CutPicture[n * n - 1], gRenderer, posIMG[0].first, posIMG[0].second);
    for (int i = 1; i < n * n; i++)
    {
        Number[i] = new GameObject(CutPicture[i - 1], gRenderer, posIMG[i].first, posIMG[i].second);
    }
    for (int i = 0; i < CutPicture.size(); ++i)
    {
        SDL_DestroyTexture(largeTexture);
        CutPicture[i] = NULL;
    }
    CutPicture.clear();
}

void Gameplay::PressReload()
{
    if (Mode == 1) Random(Height);
    else SetNguoc(Height);
    KQ.clear();
    CLOSE.clear();
    while (!OPEN.empty()) {
        OPEN.pop();
    }
    res = 1;
    checkmove = true;
    step = 0;
    if (timer.isStarted())
        timer.stop();
    isPressReload = false;
    WinnerScreenOff = false;
    UsedtoPressAutoRun = false;
}

void Gameplay::handleEvents() {
    //Hàm xử lí sự kiện để chơi trò chơi
    bool isQuit = false;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        isQuit = true;
        outGame = true;
        break;
    case SDL_KEYDOWN:
    {
        // Khi nhấn xuống phím bất kì thì ta sẽ tìm tọa độ vị trí ô trống
        if (!checksolve && !CheckGoal(a))
        {
            pair<int, int> pa = Pos(0, a);
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
            case SDLK_w:
            {
                // nếu phím được nhấn là phím Up thì ta xét vị trí dưới ô trống, nếu vị trí đó hợp lí thì hoán đổi 2 ô
                int x = a[pa.first + 1][pa.second];
                if (x != n * n) {
                    swap(a[pa.first + 1][pa.second], a[pa.first][pa.second]);
                    swap(posIMG[x], posIMG[0]);
                    Poszero = { pa.first + 1, pa.second };
                    if (!checksolve)
                    {
                        ++step;
                        if (!timer.isStarted())
                            timer.start();
                        else if (timer.isPaused())
                        {
                            timer.unpause();
                        }
                    }
                    Mix_PlayChannel(-1, gSlide, 0);
                    checkmove = true;
                }
                break;
            }
            case SDLK_DOWN:
            case SDLK_s:
            {
                int x = a[pa.first - 1][pa.second];
                if (x != n * n) {
                    swap(a[pa.first - 1][pa.second], a[pa.first][pa.second]);
                    swap(posIMG[x], posIMG[0]);
                    Poszero = { pa.first - 1, pa.second };
                    if (!checksolve)
                    {
                        ++step;
                        if (!timer.isStarted())
                            timer.start();
                        else if (timer.isPaused())
                        {
                            timer.unpause();
                        }
                    }
                    Mix_PlayChannel(-1, gSlide, 0);
                    checkmove = true;
                }
                break;
            }
            case SDLK_LEFT:
            case SDLK_a:
            {
                int x = a[pa.first][pa.second + 1];
                if (x != n * n) {
                    swap(a[pa.first][pa.second + 1], a[pa.first][pa.second]);
                    swap(posIMG[x], posIMG[0]);
                    Poszero = { pa.first, pa.second + 1 };
                    if (!checksolve)
                    {
                        ++step;
                        if (!timer.isStarted())
                            timer.start();
                        else if (timer.isPaused())
                        {
                            timer.unpause();
                        }
                    }
                    Mix_PlayChannel(-1, gSlide, 0);
                    checkmove = true;
                }
                break;
            }
            case SDLK_RIGHT:
            case SDLK_d:
            {
                int x = a[pa.first][pa.second - 1];
                if (x != n * n) {
                    swap(a[pa.first][pa.second - 1], a[pa.first][pa.second]);
                    swap(posIMG[x], posIMG[0]);
                    Poszero = { pa.first, pa.second - 1 };
                    if (!checksolve)
                    {
                        ++step;
                        if (!timer.isStarted())
                            timer.start();
                        else if (timer.isPaused())
                        {
                            timer.unpause();
                        }
                    }
                    Mix_PlayChannel(-1, gSlide, 0);
                    checkmove = true;
                }
                break;
            }
            default:
                break;
            }
        }
    }
    case SDL_MOUSEBUTTONDOWN:
    {
            // Xử lí sự kiện bằng chuột, lấy tọa độ của vị trí chuột nhấn vào (x, y)
        int x = event.motion.x, y = event.motion.y;
        //Auto Run
        if (x >= 870 && x <= 1223 && y >= 523 && y <= 590 && !CheckGoal(a))
        {
            UsedtoPressAutoRun = true;
            if (!checksolve)
            {
                checksolve = 1;
                if (timer.isStarted() && !timer.isPaused())
                    timer.pause();
            }
            else
            {
                checksolve = 0;
            }
            break;
        }
        //Button Reload
        else if(((x >= 987 && x <= 1809 && y >= 371 && y <= 447 ) || isPressReload) && checksolve == 0)
        {
            PressReload();
            break;
        }
        //Button Back
        else if (x >= 1207 && x <= 1278 && y >= 0 && y <= 71)
        {
            //cout << "O day\n";
            checksolve = 0;
            isUSE = true;
            isQuit = true;
            isRunning = false;
            isPressBack = true;
            Outfile();
            if (timer.isStarted() || timer.isPaused())
            {
                timer.stop();
            }
        }
        if (!checksolve && !CheckGoal(a))
        {
            cout << x << " " << y << endl;
             // Tính toán xem vị trí click đang nằm ở ô nào ( đưa về vị trí lúc đầu ta xét để dễ tính )
             int P = checkPos({ x, y });
             if (P)
             {
                SolveMouse(Pos(P, a));
                if (timer.isPaused())
                {
                     timer.unpause();
                }
                checkmove = true;
                if (isMoved)
                {
                    Mix_PlayChannel(-1, gSlide, 0);
                    isMoved = false;
                }
             }
             break;
            }
        }
            break;
    default:
        break;
    }
    if (CheckGoal(a) && !timer.isPaused()) timer.pause();
    // Nếu trạng thái hiện tại là trạng thái đích thì kết thúc vòng lặp, đánh giấu trò chơi đã dừng
    isRunning = !CheckGoal(a) && !isQuit && !outGame;
}

void Gameplay::SolveGame()
{
    // Hàm giải game bằng AI
    if (checkmove)
    {
        KQ.clear();
        CLOSE.clear();
        while (!OPEN.empty()) {
            OPEN.pop();
        }
        res = 1;
        bool CheckQuit = false;
        AuToRun(CheckQuit);
        if (CheckQuit)
        {
            cout << "Da thoat\n";
            return;
        }
        else cout << "Sai r\n";

    }
    checkmove = false;
    update();
    render();
    SDL_Delay(250);
    bool isQuit = false;
    SDL_Event e;
    while(index && !isQuit && !outGame)
    {
        //HandleAuto();
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
            {
                isQuit = true;
                outGame = true;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                outGame = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = e.motion.x, y = e.motion.y;
                if (x >= 870 && x <= 1223 && y >= 523 && y <= 590 && !CheckGoal(a))
                {
                    if (!checksolve)
                    {
                        checksolve = 1;
                        if (timer.isStarted() && !timer.isPaused())
                            timer.pause();
                    }
                    else
                    {
                        checksolve = 0;
                    }
                    break;
                }
                //Button Back
                else if (x >= 1207 && x <= 1278 && y >= 0 && y <= 71)
                {
                    checksolve = 0;
                    isUSE = true;
                    isQuit = true;
                    isRunning = false;
                    isPressBack = true;
                    if (timer.isStarted() || timer.isPaused())
                    {
                        timer.stop();
                    }
                }
            }
        if (!checksolve)
        {
            return;
        }
        }
        // Tìm vị trị trong mảng KQ và cập nhật lại các ảnh của trạng thái đang xét đến
        int x = FRINGE[KQ[index - 1]].second;
        cout << index << " " << x << endl;
        //display(a);
        Solve(Pos(x, a));
        if (isMoved)
        {
            Mix_PlayChannel(-1, gSlide, 0);
            isMoved = true;
        }
        update();
        render();
        SDL_Delay(250);
        index--;
    }
    checksolve = false;
    update();
    render();

}

void Gameplay::update() {
    // Hàm cập nhật lại các tọa độ của các đối tượng của game ( các mảnh puzzle )
    for (int i = 0; i <= n * n - 1; i++)
    {
        Number[i]->Update(posIMG[i].first, posIMG[i].second, Height / n, Height  / n, 589 / n, 589 / n);
    }
}

void Gameplay::render() {
    // Hàm hiển thị hình ảnh các đối tượng của game ra màn hình ( các mảnh puzzle )
    SDL_RenderClear(gRenderer);
    GoalImage.Resize(234, 234);
    Background.render(0, 0);
    GoalImage.render(928, 52);

    if (!checksolve)
        gButtonReload.HandleEvent(&event);
    gButtonReload.render(ButtonReload, ButtonReloadRect);

    if (!checksolve)
        gButtonBack.HandleEvent(&event);
    gButtonBack.render(ButtonBack, ButtonBackRect);

    if (checksolve == 1 && !CheckGoal(a))
        gButtonAutoRun.render(ButtonAutoRun, ButtonAutoRunRect, TRUE);
    else 
        gButtonAutoRun.render(ButtonAutoRun, ButtonAutoRunRect);

    StepTexture.loadFromRenderedText(to_string(step), { 0xFF, 0xFF, 0xFF }, 20);
    StepTexture.render(660, 28);
    
    timeplay = millisecondsToTimeString(timer.getTicks());
    timing.loadFromRenderedText(timeplay, { 0xFF, 0xFF, 0xFF }, 20);
    timing.render(92, 28);
    ButtonMode.render(282, 6, &ButtonModeRect[n - 3]);

    for (int i = 1; i <= n * n - 1; i++)
    {
        Number[i]->Render();
    }

    if (CheckGoal(a))
    {
        //if (!AddScoretoFile())
        //{
        //    cout << "Can't add to file \n";
        //}
        Number[0]->Render();
        SDL_RenderPresent(gRenderer);
        if (!WinnerScreenOff && !UsedtoPressAutoRun)
        {
            if (!AddScoretoFile())
            {
                cout << "Khong the luu diem vo file" << endl;
            }
            int rank = LB.ReturnRank(n - 3, { "Linh", timeplay, step });
            Winner W(step, timeplay, 10);
            W.run();
            isPressBack = W.GetIsPressBack();
            isPressReload = W.GetIsPressReload();
            cout << isPressReload;
            if (isPressReload)
            {
                PressReload();
                isPressReload = false;
            }
            else WinnerScreenOff = true;
        }
    }
    if (isPressReload)
        render();
    SDL_RenderPresent(gRenderer);
}

void Gameplay::Play()
{
    // Hàm để chơi game bằng bàn phím và chuột
    if (Mix_PlayingMusic() == 0)
    {
        //Play the music
        Mix_PlayMusic(gSoundTrack, -1);
    }
    while (running() && !outGame && !isPressBack) {
        handleEvents();
        if (outGame) break;
        if (checksolve)
        {
            //Nếu thời gian đang chạy, dừng thời gian
            if (!timer.isPaused())
                timer.pause();
            SolveGame();
        }
        isRunning = true;
        update();
        render(); 
    }
    isPressBack = false;
    cout << "End roi nha !!! " << endl;
}

void Gameplay::clean() {
    // Hàm giải phóng các đối tượng đã tạo
    delete[] Number;
    Number = NULL;
    delete[] StartGame;
    StartGame = NULL;
    Mix_FreeMusic(gSoundTrack);
    gSoundTrack = NULL;
    Mix_FreeChunk(gSlide);
    gSlide = NULL;
    ButtonBack.free();
    ButtonReload.free();
    ButtonAutoRun.free();
    ButtonMode.free();
    Background.free();
    GoalImage.free();
    SDL_DestroyTexture(PlayerTex);
    PlayerTex = NULL;
}


void Gameplay::Run() {
    if (LoadMedia())
    {
        SetUpGame(594);
        Play();
        Clear();
        clean();
    }
}