#include <onix/interrupt.h>
#include <onix/io.h>
#include <onix/assert.h>
#include <onix/debug.h>

#define LOGK(fmt,args...) DEBUGK(fmt,##args)

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_CTRL_PORT 0x64

#define INV 0 // 不可见字符

#define CODE_PRINT_SCREEN_DOWN 0xB7

typedef enum
{
    KEY_NONE,
    KEY_ESC,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    KEY_MINUS,
    KEY_EQUAL,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_Q,
    KEY_W,
    KEY_E,
    KEY_R,
    KEY_T,
    KEY_Y,
    KEY_U,
    KEY_I,
    KEY_O,
    KEY_P,
    KEY_BRACKET_L,
    KEY_BRACKET_R,
    KEY_ENTER,
    KEY_CTRL_L,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_SEMICOLON,
    KEY_QUOTE,
    KEY_BACKQUOTE,
    KEY_SHIFT_L,
    KEY_BACKSLASH,
    KEY_Z,
    KEY_X,
    KEY_C,
    KEY_V,
    KEY_B,
    KEY_N,
    KEY_M,
    KEY_COMMA,
    KEY_POINT,
    KEY_SLASH,
    KEY_SHIFT_R,
    KEY_STAR,
    KEY_ALT_L,
    KEY_SPACE,
    KEY_CAPSLOCK,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_NUMLOCK,
    KEY_SCRLOCK,
    KEY_PAD_7,
    KEY_PAD_8,
    KEY_PAD_9,
    KEY_PAD_MINUS,
    KEY_PAD_4,
    KEY_PAD_5,
    KEY_PAD_6,
    KEY_PAD_PLUS,
    KEY_PAD_1,
    KEY_PAD_2,
    KEY_PAD_3,
    KEY_PAD_0,
    KEY_PAD_POINT,
    KEY_54,
    KEY_55,
    KEY_56,
    KEY_F11,
    KEY_F12,
    KEY_59,
    KEY_WIN_L,
    KEY_WIN_R,
    KEY_CLIPBOARD,
    KEY_5D,
    KEY_5E,

    // 以下位自定义按键, 为和 keymap 索引匹配
    KEY_PRINT_SCREEN,
}KEY;

static char keymap[][4] =
{
    /* 扫描码 未与 shift 组合 与 shift 组合 以及相关*/
    /* 0x00 */ {INV,INV,false,false},
    /* 0x01 */ {0x1b,0x1b,false,false},
    /* 0x02 */ {'1','!',false,false},
    /* 0x03 */ {'2','@',false,false},
    /* 0x04 */ {'3','#',false,false},
    /* 0x05 */ {'4','$',false,false},
    /* 0x06 */ {'5','%',false,false},
    /* 0x07 */ {'6','^',false,false},
    /* 0x08 */ {'7','&',false,false},
    /* 0x09 */ {'8','*',false,false},
    /* 0x0a */ {'9','(',false,false},
    /* 0x0b */ {'0',')',false,false},
    /* 0x0c */ {'-','_',false,false},
    /* 0x0d */ {'=','+',false,false},
    /* 0x0e */ {'\b','\b',false,false},
    /* 0x0f */ {'\t','\t',false,false},
    /* 0x10 */ {'q','Q',false,false},
    /* 0x11 */ {'w','W',false,false},
    /* 0x12 */ {'e','E',false,false},
    /* 0x13 */ {'r','R',false,false},
    /* 0x14 */ {'t','T',false,false},
    /* 0x15 */ {'y','Y',false,false},
    /* 0x16 */ {'u','U',false,false},
    /* 0x17 */ {'i','I',false,false},
    /* 0x18 */ {'o','O',false,false},
    /* 0x19 */ {'p','P',false,false},
    /* 0x1a */ {'[','{',false,false},
    /* 0x1b */ {']','}',false,false},
    /* 0x1c */ {'\n','\n',false,false},
    /* 0x1d */ {INV,INV,false,false},
    /* 0x1e */ {'a','A',false,false},
    /* 0x1f */ {'s','S',false,false},
    /* 0x20 */ {'d','D',false,false},
    /* 0x21 */ {'f','F',false,false},
    /* 0x22 */ {'g','G',false,false},
    /* 0x23 */ {'h','H',false,false},
    /* 0x24 */ {'j','J',false,false},
    /* 0x25 */ {'k','K',false,false},
    /* 0x26 */ {'l','L',false,false},
    /* 0x27 */ {';',':',false,false},
    /* 0x28 */ {'\'', '"',false,false},
    /* 0x29 */ {'`', '~',false,false},
    /* 0x2a */ {INV,INV,false,false},
    /* 0x2b */ {'\\', '|',false,false},
    /* 0x2c */ {'z','Z',false,false},
    /* 0x2d */ {'x','X',false,false},
    /* 0x2e */ {'c','C',false,false},
    /* 0x2f */ {'v','V',false,false},
    /* 0x30 */ {'b','B',false,false},
    /* 0x31 */ {'n','N',false,false},
    /* 0x32 */ {'m','M',false,false},
    /* 0x33 */ {',', '<',false,false},
    /* 0x34 */ {'.', '>',false,false},
    /* 0x35 */ {'/', '?',false,false},
    /* 0x36 */ {INV,INV,false,false},
    /* 0x37 */ {'*','*',false,false},
    /* 0x38 */ {INV,INV,false,false},
    /* 0x39 */ {' ',' ',false,false},
    /* 0x3a */ {INV,INV,false,false},
    /* 0x3b */ {INV,INV,false,false},
    /* 0x3c */ {INV,INV,false,false},
    /* 0x3d */ {INV,INV,false,false},
    /* 0x3e */ {INV,INV,false,false},
    /* 0x3f */ {INV,INV,false,false},
    /* 0x40 */ {INV,INV,false,false},
    /* 0x41 */ {INV,INV,false,false},
    /* 0x42 */ {INV,INV,false,false},
    /* 0x43 */ {INV,INV,false,false},
    /* 0x44 */ {INV,INV,false,false},
    /* 0x45 */ {INV,INV,false,false},
    /* 0x46 */ {INV,INV,false,false},
    /* 0x47 */ {'7',INV,false,false},
    /* 0x48 */ {'8',INV,false,false},
    /* 0x49 */ {'9',INV,false,false},
    /* 0x4a */ {'-','-',false,false},
    /* 0x4b */ {'4',INV,false,false},
    /* 0x4c */ {'5',INV,false,false},
    /* 0x4d */ {'6',INV,false,false},
    /* 0x4e */ {'+','+',false,false},
    /* 0x4f */ {'1',INV,false,false},
    /* 0x50 */ {'2',INV,false,false},
    /* 0x51 */ {'3',INV,false,false},
    /* 0x52 */ {'0',INV,false,false},
    /* 0x53 */ {'.',0x7F,false,false},   // pad . - Delete
    /* 0x54 */ {INV,INV,false,false},
    /* 0x55 */ {INV,INV,false,false},
    /* 0x56 */ {INV,INV,false,false},
    /* 0x57 */ {INV,INV,false,false},
    /* 0x58 */ {INV,INV,false,false},
    /* 0x59 */ {INV,INV,false,false},
    /* 0x5a */ {INV,INV,false,false},
    /* 0x5b */ {INV,INV,false,false},
    /* 0x5c */ {INV,INV,false,false},
    /* 0x5d */ {INV,INV,false,false},
    /* 0x5e */ {INV,INV,false,false},

    // Print Screen 是强制定义 本身为 0xB7
    /* 0x5f */ {INV,INV,false,false}, //Print Screen
};

static bool capslock_state; // 大写锁定
static bool scrlock_state;  // 滚动锁定
static bool numlock_state;  // 数字锁定
static bool extcode_state;  // 扩展码状态

// CTRL 键状态
#define ctrl_state (keymap[KEY_CTRL_L][2] || keymap[KEY_CTRL_L][3])

// ALT 键状态
#define alt_state (keymap[KEY_ALT_L][2] || keymap[KEY_ALT_L][3])

// shift 键状态
#define shift_state (keymap[KEY_SHIFT_L][2] || keymap[KEY_SHIFT_R][2])

void keyboard_handler(int vector)
{ 
    assert(vector == 0x21);
    send_eoi(vector);                       // 发送中断结束信号

    // 接收扫描码
    u16 scancode = inb(KEYBOARD_DATA_PORT);
    u8 ext =2; // keymap 状态索引，默认没有shift 键

    // 是扩展码字节
    if (scancode == 0xe0)
    {
        // 置扩展状态
        extcode_state = true;
        return;
    }

    // 是扩展码
    if (extcode_state)
    {
        // 改状态索引
        ext = 3;

        // 修改扫描码，添加 0xe0 前缀
        scancode |= 0xe000;

        // 扩展状态无效
        extcode_state = false;
    }

    // 获得通码
    u16 makecode =(scancode & 0x7f);
    if (makecode ==CODE_PRINT_SCREEN_DOWN)
    {
        makecode =KEY_PRINT_SCREEN;
    }
    
    // 生成码非法
    if (makecode >KEY_PRINT_SCREEN)
    {
        return;
    }

    // DEBUGK("scancode 0x%x\n",scancode);
    
    // 是否为断码，按键抬起
    bool breakcode =((scancode & 0x80) != 0);
    if (breakcode)
    {
        // 如果是则设置状态
        keymap[makecode][ext] =false;
        return;
    }
    
    // 下面是通码，按键按下
    keymap[makecode][ext] =true;

    // 是否需要设置 LED 灯
    bool led =false;
    if (makecode== KEY_NUMLOCK)
    {
        numlock_state = !numlock_state;
        led =true;
    }
    else if (makecode== KEY_CAPSLOCK)
    {
        capslock_state = !capslock_state;
        led =true;
    }
    else if (makecode== KEY_SCRLOCK)
    {
        scrlock_state = !scrlock_state;
        led =true;
    }

    // 计算 shift 状态
    bool shift =false;
    if (capslock_state)
    {
        shift =!shift;
    }
    if (shift_state)
    {
        shift =!shift;
    }

    // 获得按键 ASCII 码
    char ch =0;
    // [/?] 这个键比较特殊，只有这个键扩展码和普通码一样
    if (ext ==3 && (makecode!=KEY_SLASH))
    {
        ch =keymap[makecode][1];
    }
    else
    {
        ch =keymap[makecode][shift];
    }
    
    if (ch==INV)
        return;
    
    LOGK("keydown: %c\n",ch);
}

void keyboard_init()
{
    numlock_state = false;
    scrlock_state = false;
    capslock_state = false;
    extcode_state = false;

    set_interrupt_handler(IRQ_KEYBOARD,keyboard_handler);
    set_interrupt_mask(IRQ_KEYBOARD,true);
}
