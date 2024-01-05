#include <stdlib.h>
#include <swilib.h>

typedef struct {
	// Сам CSM
	CSM_RAM csm;
	// Далее идут любые пользовательские данные
	int gui_id;
} MAIN_CSM;

typedef struct {
	// Сам GUI
	GUI base;
	// Далее идут любые пользовательские данные
	WSHDR *ws;
	int page;
} MAIN_GUI;

static void example_basic_canvas(MAIN_GUI *gui);
static void example_draw_text(MAIN_GUI *gui);

/*
 * GUI (канвас)
 * */

// Конструктор GUI
static void gui_oncreate(GUI *data, malloc_func_t malloc_fn) {
	MAIN_GUI *gui = (MAIN_GUI *) data;
	
	// Статус GUI нужно обновлять вручную
	gui->base.state = CSM_GUI_STATE_UNFOCUSED;
	
	// Инициализируем какие-то свои данные
	gui->ws = AllocWS(4096 * 10);
}

// Вызывается при каждом вызову REDRAW()
// Тут можно рисовать свой GUI
static void gui_onredraw(GUI *data) {
	MAIN_GUI *gui = (MAIN_GUI *) data;
	
	switch ((gui->page % 2)) {
		case 0:
			example_basic_canvas(gui);
		break;
		
		case 1:
			example_draw_text(gui);
		break;
	}
}

#define UTF16_ALIGN_NONE		0xE01C
#define UTF16_ALIGN_RIGHT		0xE01D // TEXT_ALIGNRIGHT
#define UTF16_ALIGN_LEFT		0xE01E // TEXT_ALIGNLEFT
#define UTF16_ALIGN_CENTER		0xE01F // TEXT_ALIGNMIDDLE

static void example_draw_text(MAIN_GUI *gui) {
	char black[] = { 0x00, 0x00, 0x00, 0x64 };
	char grey[] = { 0xCC, 0xCC, 0xCC, 0x64 };
	char pink[] = { 0xFF, 0x00, 0xFF, 0x64 / 2};
	char green[] = { 0x00, 0xFF, 0x00, 0x64 };
	
	DrawRoundedFrame(0, 0, ScreenW() - 1, ScreenH() - 1, 0, 0, 0, grey, grey);
	
	uint16_t body[] = {
		UTF16_ALIGN_LEFT,
		'l', 'e', 'f', 't', UTF16_NO_UNDERLINE, '!', UTF16_UNDERLINE,
		UTF16_ALIGN_RIGHT,
		'r', 'i', 'g', 'h', 't',
		UTF16_ALIGN_LEFT,
		'l', 'e', 'f', 't',
		UTF16_ALIGN_CENTER,
		'c', 'e', 'n', 't', 'e', 'r',
		'\n',
		UTF16_ALIGN_NONE,
		'l', 'e', 'f', 't',
		UTF16_FONT_SMALL,
		UTF16_ALIGN_CENTER,
		'c', 'e', 'n', 't', 'e', 'r',
		UTF16_ALIGN_RIGHT,
		UTF16_FONT_MEDIUM,
		'r', 'i', 'g', 'h', 't', ' ',
		'r', 'i', 'g', 'h', 't', ' ',
		'r', 'i', 'g', 'h', 't', ' ',
		'r', 'i', 'g', 'h', 't', ' ',
		'r', 'i', 'g', 'h', 't', ' ',
		UTF16_ALIGN_LEFT,
		
		UTF16_TEXT_COLOR_RGBA, 0xFF00, 0x0064, 'R', 'R',
		UTF16_TEXT_COLOR_RGBA, 0x00FF, 0x0064, 'G', 'G',
		UTF16_TEXT_COLOR_RGBA, 0x0000, 0xFF64, 'B', 'B',
		
		UTF16_BG_INVERTION2,
		
		UTF16_TEXT_COLOR_RGBA, 0xFF00, 0x0064, 'R', 'R',
		UTF16_TEXT_COLOR_RGBA, 0x00FF, 0x0064, 'G', 'G',
		UTF16_TEXT_COLOR_RGBA, 0x0000, 0xFF64, 'B', 'B',
		
		UTF16_NO_INVERTION, UTF16_NO_UNDERLINE,
		0xE100, // icon
	};
	
	memcpy(gui->ws->wsbody + 1, body, sizeof(body));
	gui->ws->wsbody[0] = sizeof(body) / 2;
	
	DrawString(gui->ws, 0, 5, ScreenW() - 1, ScreenH() - 1, FONT_SMALL, TEXT_UNDERLINE, black, pink);
	
	wsprintf(gui->ws, "%t", "test linear str\ntest2");
	DrawScrollString(gui->ws, 0, ScreenH() - 30, ScreenW() - 1, ScreenH() - 1, 2, FONT_SMALL, TEXT_UNDERLINE, black, pink);
}

static void example_basic_canvas(MAIN_GUI *gui) {
	char black[] = { 0x00, 0x00, 0x00, 0x64 };
	char grey[] = { 0xCC, 0xCC, 0xCC, 0x64 };
	char pink[] = { 0xFF, 0x00, 0xFF, 0x64 / 2};
	char green[] = { 0x00, 0xFF, 0x00, 0x64 };
	
	// Фон
	DrawRoundedFrame(0, 0, ScreenW() - 1, ScreenH() - 1, 0, 0, 0, grey, grey);
	
	int x = 5;
	int y = 5;

#ifdef ELKA
	int w = 50;
	int h = 70;
	int pic_zero = 616;
#else
	int w = 50 / 2;
	int h = 70 / 2;
	int pic_zero = 608;
#endif
	
	// RECT_DOT_OUTLINE - stroke dotted
	int arc_flags = 0;

	// Pie
	drawArc(x, y, x + w, y + h, 35, 150, arc_flags, black, pink);
	x += w + 5;
	
	// Arc
	drawArc(x, y, x + w, y + h, 35, 150, arc_flags, black, NULL);
	x += w + 5;
	
	// ---------------------------------------
	
	// RECT_DOT_OUTLINE - stroke dotted
	// RECT_FILL_WITH_PEN - bg dotted with fg color
	// RECT_DRAW_INVERT - invert bg
	int rect_flags = 0;
	
	// fill rect
	DrawRectangle(x, y, x + w, y + h, rect_flags, black, pink);
	x += w + 5;
	
	// stroke rect
	DrawRectangle(x, y, x + w, y + h, rect_flags, black, NULL);
	x += w + 5;
	
	x = 5;
	y += h + 5;
	
	// ---------------------------------------
	
	// RECT_DOT_OUTLINE - stroke dotted
	int round_rect_flags = 0;
	
	// fill rounded rect
	DrawRoundedFrame(x, y, x + w, y + h, 30, 15, round_rect_flags, black, pink);
	x += w + 5;
	
	// stroke rounded rect
	DrawRoundedFrame(x, y, x + w, y + h, 30, 15, round_rect_flags, black, NULL);
	x += w + 5;
	
	// ---------------------------------------
	
	// fill triangle
	fillTriangle(x, y, x + w / 2, y + h, x + w, y, round_rect_flags, black, pink);
	x += w + 5;
	
	// stroke triangle
	fillTriangle(x, y, x + w / 2, y + h, x + w, y, round_rect_flags, black, NULL);
	x += w + 5;
	
	// ---------------------------------------
	
	x = 5;
	y += h + 5;
	
	// Picture from PIT
	DrawImg(x, y, pic_zero);
	x += GetImgWidth(pic_zero) + 5;
	
	// Picture from PIT
	DrawImgBW(x, y, pic_zero, pink, green);
	x += GetImgWidth(pic_zero) + 5;
	
	// ---------------------------------------
	
	// LINE_DOTTED
	int line_flags = 0;
	
	// Line
	DrawLine(x, y, x + w, y + h, line_flags, pink);
	DrawLine(x, y + 10, x + w, y + h, line_flags, pink);
	DrawLine(x, y + 20, x + w, y + h, line_flags, pink);
	DrawLine(x, y + 30, x + w, y + h, line_flags, pink);
}

// Вызывается как только GUI становится виден на экране
static void gui_onfocus(GUI *data, malloc_func_t malloc_fn, mfree_func_t mfree_fn) {
	MAIN_GUI *gui = (MAIN_GUI *) data;
	
	// Статус GUI нужно обновлять вручную
	gui->base.state = CSM_GUI_STATE_FOCUSED;
	
	// Запрещаем выходить в IDLE по таймауту
	DisableIDLETMR();
	
#ifdef ELKA
	// Запрещаем рисовать иконбар с временем, уровнем сети и т.д. на ёлке
	DisableIconBar(1);
#endif
}

// Вызывается когда GUI перестаёт быть виден на экране
static void gui_onunfocus(GUI *data, mfree_func_t mfree_fn) {
	MAIN_GUI *gui = (MAIN_GUI *) data;
	
	// Вполняем код только если GUI до этого был в фокусе
	// Похоже, что может быть ложный вызов onunfocus
	if (gui->base.state != CSM_GUI_STATE_FOCUSED)
		return;
	
	// Статус GUI нужно обновлять вручную
	gui->base.state = CSM_GUI_STATE_UNFOCUSED;
	
	// Разрешаем уходить в IDLE по таймауту
	RestartIDLETMR();
	
#ifdef ELKA
	// Разрешаем рисовать иконбар с временем, уровнем сети и т.д. на ёлке
	DisableIconBar(0);
#endif
}

// Вызывается при нажатии кнопок
static int gui_onkey(GUI *data, GUI_MSG *msg) {
	MAIN_GUI *gui = (MAIN_GUI *) data;
	
	if ((msg->gbsmsg->msg == KEY_DOWN || msg->gbsmsg->msg == LONG_PRESS)) {
		switch (msg->gbsmsg->submess) {
			case RIGHT_SOFT:
				// Если вернуть 1 в этом методе, то GUI будет закрыт
			return 1;
			
			case '5':
				// Если нажали кнопку '5', перерисуем экран
				gui->page++;
				REDRAW();
			break;
			
			case '1':
				// Если нажали кнопку '1', выведем алерт
				ShowMSG(1, (int) "Текст в cp1251!");
			break;
		}
	}
	return 0;
}

// Деструктор GUI
static void gui_onclose(GUI *data, mfree_func_t mfree_fn) {
	MAIN_GUI *gui = (MAIN_GUI *) data;
	
	// Статус GUI нужно обновлять вручную
	gui->base.state = CSM_GUI_STATE_CLOSED;
	
	// Очищаем какие-то свои данные
	FreeWS(gui->ws);
}

// Хз что делает
static int gui_method8(void) {
	return 0;
}

// Хз что делает
static int gui_method9(void) {
	return 0;
}

// Методы GUI
static GUI_METHODS gui_methods = {
	gui_onredraw,
	gui_oncreate,
	gui_onclose,
	gui_onfocus,
	gui_onunfocus,
	gui_onkey,
	NULL,
	kill_data, // onDestroy, вызывается для очистки памяти, можно просто использовать kill_data
	gui_method8,
	gui_method9,
	NULL,
};

// Область экрана которую занимает GUI
static RECT gui_rect;

/*
 * CSM (окно)
 * */

// Вызывается при создании CSM
static void maincsm_oncreate(CSM_RAM *data) {
	MAIN_CSM *csm = (MAIN_CSM *) data;
	
	csm->csm.state = CSM_STATE_OPEN;
	csm->csm.unk1 = 0;
	
	// Задаём границы канваса (рисуем на весь экран)
	StoreXYWHtoRECT(&gui_rect, 0, 0, ScreenW(), ScreenH());
	
	// Создаём наш GUI и запомнияем его ID в csm->gui_id
	MAIN_GUI *gui = malloc(sizeof(MAIN_GUI));
	zeromem(gui, sizeof(MAIN_GUI));
	gui->base.canvas = &gui_rect;
	gui->base.methods = &gui_methods;
	gui->base.item_ll.data_mfree = mfree_adr();
	
	csm->gui_id = CreateGUI(gui);
}

// Вызывается при закрытии CSM
static void maincsm_onclose(CSM_RAM *csm) {
	// Убиваем эльф (особождается память занятая ELF'ом)
	kill_elf();
}

// Вызывается для всех GBS-сообщений в MMI процесс
static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg) {
	MAIN_CSM *csm = (MAIN_CSM *) data;
	if ((msg->msg == MSG_GUI_DESTROYED) && ((int) msg->data0 == csm->gui_id)) {
		// Если наш GUI закрылся, переводим CSM в состояние "closed"
		// После этого он будет автоматически закрыт
		csm->csm.state = CSM_STATE_CLOSED;
	}
	return 1;
}

// Структура, которая описывает CSM
static const int minus11 = -11; // Что-то загадочное, но важное :D
static uint16_t maincsm_name_body[140]; // Память для названия CSM

static const struct {
	CSM_DESC maincsm; // Это сам CSM
	WSHDR maincsm_name; // Это специальная строка для XTask с именем CSM
} MAINCSM = {
	.maincsm = {
		maincsm_onmessage,
		maincsm_oncreate,
#ifdef NEWSGOLD
		0, 0, 0, 0, 
#endif
		maincsm_onclose,
		sizeof(MAIN_CSM),
		1, &minus11
	},
	.maincsm_name = { maincsm_name_body, NAMECSM_MAGIC1, NAMECSM_MAGIC2, 0x0, sizeof(maincsm_name_body) - 1, 0}
};

// Можно динамически изменять имя CSM, XTask это сразу увидит
static void UpdateCSMname(void) {
	wsprintf(&MAINCSM.maincsm_name, "Hello World CSM");
}

/*
 * Main
 * */

// exe - путь к запущенному эльфу
// fname - путь к файлу, если эльф был запущен из эксплорера для файла с зарегистрированным расширением
int main(char *exe, char *fname, void *p1) {
	// Создаём CSM, это что-то вроде окна
	MAIN_CSM main_csm;
	LockSched();
	UpdateCSMname();
	CreateCSM(&MAINCSM.maincsm, &main_csm, 0);
	UnlockSched();
	
	// После выхода из main эльф продолжает жить
	// Пока не будет вызван kill_elf()
	return 0;
}
