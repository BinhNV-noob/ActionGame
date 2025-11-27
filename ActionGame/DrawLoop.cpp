#include <windows.h>
#include "Def.h"
#include "Work.h"

void DrawLoop(void)
{
    HDC hDC;
    HDC hDCWork;
    int i;

    // Lấy DC của cửa sổ chính
    hDC = GetDC(hwnd);

    // Tạo DC ảo để vẽ bitmap
    hDCWork = CreateCompatibleDC(hDC);

    
    // Phân nhánh xử lý vẽ theo chế độ màn hình (mode)
    // gmode = 1 : Màn hình TITLE
    // gmode = 2 : Màn hình GAME
    
    switch (gmode)
    {

        
        // CASE 1: MÀN HÌNH TITLE
        
    case 1:

        // Vẽ nền Title
        SelectObject(hDCWork, hBmpTbl[BMP_TITLE]);
        BitBlt(hDCBack, 0, 0, WINDOW_W, WINDOW_H, hDCWork, 0, 0, SRCCOPY);

        // Hiển thị chữ START nhấp nháy
        if ((gameCount % 20) < 10)
        {
            SelectObject(hDCWork, hBmpTbl[BMP_START]);

            // Vẽ phần mask (nửa dưới)
            BitBlt(hDCBack,
                WINDOW_W / 2 - (START_W / 2),
                WINDOW_H - START_H - 100,
                START_W, START_H,
                hDCWork, 0, 40,
                SRCAND);

            // Vẽ phần hình ảnh thật (nửa trên)
            BitBlt(hDCBack,
                WINDOW_W / 2 - (START_W / 2),
                WINDOW_H - START_H - 100,
                START_W, START_H,
                hDCWork, 0, 0,
                SRCPAINT);
        }
        break;

        
        // CASE 2: MÀN HÌNH GAME
        
    case 2:

        // Vẽ background game
        SelectObject(hDCWork, hBmpTbl[BMP_BG]);
        BitBlt(hDCBack, 0, 0, WINDOW_W, WINDOW_H, hDCWork, 0, 0, SRCCOPY);

        // Duyệt toàn bộ object trong game
        for (i = 0; i < MAXOBJ; i++)
        {
            // Chỉ vẽ object đang hoạt động và được hiển thị
            if ((obj[i].id != 0) && (obj[i].mode != 0) && (obj[i].dspf != 0))
            {
                // Chọn bitmap của object tương ứng
                SelectObject(hDCWork, hBmpTbl[obj[i].idx]);

                // Vẽ MASK (làm trong suốt)
                BitBlt(hDCBack,
                    (int)obj[i].xposition - (obj[i].xsize / 2),
                    (int)obj[i].yposition - (obj[i].ysize / 2),
                    obj[i].xsize, obj[i].ysize,
                    hDCWork,
                    obj[i].xmoff,
                    obj[i].ymoff,
                    SRCAND);

                //Vẽ COLOR
                BitBlt(hDCBack,
                    (int)obj[i].xposition - (obj[i].xsize / 2),
                    (int)obj[i].yposition - (obj[i].ysize / 2),
                    obj[i].xsize, obj[i].ysize,
                    hDCWork,
                    obj[i].xboff,
                    obj[i].yboff,
                    SRCPAINT);
            }
        }
        break;
    }

    //Xóa DC ảo
    DeleteDC(hDCWork);

    //Trả lại DC cửa sổ
    ReleaseDC(hwnd, hDC);
}
