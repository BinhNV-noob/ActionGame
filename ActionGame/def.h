//def.h

#define MAXOBJ      1000    // số lượng object / sprite tối đa
#define WINDOW_W    800     // chiều rộng cửa sổ
#define WINDOW_H    640     // chiều cao cửa sổ

//#define GM_      0       // (chưa dùng)

//#define O_       0       // (chưa dùng)

//#define ID_      1       // (chưa dùng)

#define PLAYER_W  82       // chiều rộng sprite nhân vật
#define PLAYER_H  80       // chiều cao sprite nhân vật

#define ENEMY_W   82       // chiều rộng sprite enemy
#define ENEMY_H   80       // chiều cao sprite enemy

#define START_W   256      // chiều rộng nút/ảnh START
#define START_H   40       // chiều cao nút/ảnh START

// quản lý ID (phân loại loại nhân vật / object)
#define ID_PLAYER  1
#define ID_ENEMY   ID_PLAYER + 1

// quản lý index trong mảng (chỉ số trong table / mảng CharData)
#define IDX_PLAYER  1
#define IDX_ENEMY   IDX_PLAYER + 1

// ID bitmap (index trong mảng bitmap)
#define BMP_BG          0   // background
#define BMP_PLAYER_L    1   // nhân vật quay trái
#define BMP_PLAYER_R    2   // nhân vật quay phải
#define BMP_ENEMY_L     3   // enemy quay trái
#define BMP_ENEMY_R     4   // enemy quay phải
#define BMP_TITLE       5   // màn hình title
#define BMP_START       6   // nút / chữ START
#define BMP_SCORE       7   // hiển thị SCORE
#define BMP_HOLE        8   // hố / lỗ
#define BMP_BLOCK       9   // block / chướng ngại vật

#define BLOCK_W 32
#define BLOCK_H 32

#define X_LINE (WINDOW_W / BLOCK_W) //25
#define Y_LINE (WINDOW_H/BLOCK_H) //20



#define ANIM_WAIT_TIME 10
#define ANIM_WAIT_PATTERN 4
#define ANIM_WAIT_LOOP 1
#define ANIM_WAIT_OFFSET 0

#define ANIM_WALK_TIME 10
#define ANIM_WALK_PATTERN 4
#define ANIM_WALK_LOOP 1
#define ANIM_WALK_OFFSET 4

#define KEYUP 0x01
#define KEYDOWN 0x02
#define KEYLEFT 0x04
#define KEYRIGHT 0x08
#define KEYJUMP 0x10
#define KEYATTACK 0x20


#define PLAYERMODE_WAIT 1
#define PLAYERMODE_WALK PLAYERMODE_WAIT + 1


#define PLAYER_ATK_W 89

#define ANIM_ATTACK_TIME     10
#define ANIM_ATTACK_PATTERN   2
#define ANIM_ATTACK_LOOP      0
#define ANIM_ATTACK_OFFSET   12

#define PLAYERMODE_ATTACK        PLAYERMODE_WALK + 1
#define PLAYERMODE_ATTACK_AFTER  PLAYERMODE_ATTACK + 1

#define PLAYER_ATTACK_OFFSET   968


#define PLAYER_HIT_W  25   // = 50 / 2
#define PLAYER_HIT_H  30   // = 60 / 2


// cấu trúc dữ liệu cho 1 nhân vật / object
struct CharData {
    int id;         // ID loại nhân vật (player, enemy, v.v.)
    int mode;       // trạng thái / mode hành động
    BOOL dspf;      // cờ hiển thị: 1 = vẽ, 0 = không vẽ
    int xposition;  // vị trí X (tọa độ trên màn hình / world)
    int yposition;  // vị trí Y
    double xspeed;  // vận tốc theo trục X
    double yspeed;  // vận tốc theo trục Y
    int xsize;      // kích thước theo chiều ngang (hitbox / sprite)
    int ysize;      // kích thước theo chiều dọc
    int xboff;      // offset vẽ theo X (lệch so với tâm / gốc)
    int yboff;      // offset vẽ theo Y
    int xmoff;      // mask offset X (nếu dùng mask bitmap)
    int ymoff;      // mask offset Y
    int actioncnt;  // bộ đếm frame / số tick cho hành động
    int idx;        // index hình / bitmap tương ứng (BMP_*)

    int animcnt;        // その絵の状態を保持する時間 0になったら次の絵に移行
    int animpatternnow; // 今何枚目の絵か 
    int animpattern;    // そのアニメの絵の数
    int animloop;       // そのアニメはループするタイプか 0:ループ無し 1:ループあり
};

typedef void (*TBLJP)(void); // con trỏ hàm: kiểu hàm không tham số, không trả về


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);// prototype hàm window procedure
void GameLoop(void);// loop chính của game (gọi liên tục mỗi frame)
void ActionLoop(void);// xử lý logic / cập nhật trạng thái object (di chuyển, va chạm, v.v.)

void DrawLoop(void);// vẽ toàn bộ màn hình (background, player, enemy, UI, v.v.)
void InitSet(void);// khởi tạo giá trị ban đầu (object, biến global, v.v.)

void KeyCheck(void);
void ActPlayer(void);
void ActEnemy(void);
int PlyEneHitcheck(void);

int animManager(int anim_time, int next_mode, int dispflg);

