#include "stdafx.h"
#include <mutex>
#include <thread>

Recorder *recorder = nullptr;
std::mutex lock;

void registerRecorder(Recorder *rec) {
	recorder = rec;
}

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		lock.lock();
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
		if (recorder != nullptr) {
			recorder->keyboardEvent(wParam - 0x100, p->vkCode);
		}
		lock.unlock();
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		lock.lock();
		if (wParam != 512 && recorder != nullptr) {
			recorder->mouseEvent((wParam - 0x200));
		}
		lock.unlock();
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

void messageLoop() {
	HHOOK mousehook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);
	HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);
	MSG msg;
	while (!GetMessage(&msg, NULL, NULL, NULL)) {    //this while loop keeps the hook
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(keyboardHook);
	UnhookWindowsHookEx(mousehook);
}

void runHooks() {
	std::thread messages(messageLoop);
	messages.detach();
}