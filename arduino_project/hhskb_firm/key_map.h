#pragma once

// �E��ʏ핶���̔z�u��`
static int RSymbol[ROWMAX][COLMAX] =
{
	{ '7',   '8',  '9',  '0',  '-',  '=', '\\', '`' },
	{ 'y',   'u',  'i',  'o',  'p',  '[',  ']', DELE },
	{ 'h',   'j',  'k',  'l',  ';', '\'', RET_, NASB },
	{ 'b',   'n',  'm',  ',',  '.',  '/', RSFT, Fn },
	{ SPC_, RALT, RCTL, NASB, RGUI, NASB, NASB, NASB }
};

// �E��p�t�@���N�V�����������V���{�� 
// �ʏ푤��Fn���`���Ă���ӏ��͂����ŕύX�o���܂���B
static int RFnSymbol[ROWMAX][COLMAX] =
{
	{ F7__, F8__, F9__, F10_, F11_, F12_, INST, DELE },
	{ NOP_, NOP_, PRTS, NOP_, NOP_, UP__, NOP_, BS__ },
	{ NOP_, NOP_, HOME, PUP_, LEFT, RIGT, NOP_, NASB },
	{ NOP_, NOP_, NOP_, END_, PDWN, DOWN, RSFT, Fn },
	{ SPC_, RALT, RCTL, NASB, RGUI, NASB, NASB, NASB }
};

// �E��C���L�[
// ���������Ŕ�������C���L�[�B�C���L�[�����i�[�o���܂���B
static int RModSymbol[ROWMAX][COLMAX] =
{
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_ },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_ },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_ },
	{ RCTL, RALT, NOP_, NASB, NOP_, NASB, NASB, NASB },
};

// ����p�V���{��
static int LSymbol[ROWMAX][COLMAX] =
{
	{ ESC_, '1',  '2',  '3',  '4',  '5',  '6',  NASB },
	{ TAB_, 'q',  'w',  'e',  'r',  't',  NASB, NASB },
	{ LCTL, 'a',  's',  'd',  'f',  'g',  NASB, NASB },
	{ LSFT, 'z',  'x',  'c',  'v',  'b',  NASB, NASB },
	{ NASB, NASB, LGUI, NOP_, LALT, SPC_, NASB, NASB }
};

// ����pFn�������V���{��
// �ʏ푤��Fn���`���Ă���ӏ��͂����ŕύX�o���܂���B
static int LFnSymbol[ROWMAX][COLMAX] =
{
	{ ESC_, F1__, F2__, F3__, F4__, F5__, F6__, NASB },
	{ TAB_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ LCTL, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ LSFT, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ NASB, NASB, LGUI, NOP_, LALT, LCTL, NASB, NASB }
};

// ����C���L�[
// ���������Ŕ�������C���L�[�B�C���L�[�����i�[�ł��܂���
static int LModSymbol[ROWMAX][COLMAX] =
{
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ NOP_, NOP_, NOP_, NOP_, NOP_, NOP_, NASB, NASB },
	{ NASB, NASB, NOP_, NOP_, NOP_, LCTL, NASB, NASB },
};
