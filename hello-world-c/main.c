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
} MAIN_GUI;

/*
 * GUI (канвас)
 * */

// Конструктор GUI
static void gui_oncreate(GUI *data, malloc_func_t malloc_fn) {
	MAIN_GUI *gui = (MAIN_GUI *) data;
	
	// Статус GUI нужно обновлять вручную
	gui->base.state = CSM_GUI_STATE_UNFOCUSED;
	
	// Инициализируем какие-то свои данные
	gui->ws = AllocWS(128);
}

// Вызывается при каждом вызову REDRAW()
// Тут можно рисовать свой GUI
static void gui_onredraw(GUI *data) {
	MAIN_GUI *gui = (MAIN_GUI *) data;
	
	const char *bg_color = GetPaletteAdrByColorIndex(PC_BACKGROUND);
	const char *fg_color = GetPaletteAdrByColorIndex(PC_FOREGROUND);
	
	// Что-то рисуем
	DrawRoundedFrame(0, 0, ScreenW() - 1, ScreenH() - 1, 0, 0, 0, bg_color, bg_color);
	wsprintf(gui->ws, "ram %d", GetFreeRamAvail());
	DrawString(gui->ws, 0, 28, ScreenW() - 1, ScreenH() - 1, FONT_SMALL, 2 + 32, bg_color, fg_color);
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
				REDRAW();
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
	
	// Pадаём границы канваса (рисуем на весь экран)
	StoreXYWHtoRECT(&gui_rect, 0, 0, ScreenW() - 1, ScreenH() - 1);
	
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

// Вызываемся для всех GBS-сообщений в MMI процесс
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
static uint16_t maincsm_name_body[140]; // Память для имени CSM

static const struct {
	CSM_DESC maincsm; // Это сам CSM
	WSHDR maincsm_name; // Это специальная строка для XTask с именем CSM
	// Далее могут идти любые пользовательские поля
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
