#pragma once

// hhskb_firm�Ŏg����ʓI�Ȍ^�A�萔��`

enum LorR
{
	Left,
	Right,
};
static const int OFF = 1;
static const int ON = 0;    // ����̉�H�̓A�N�e�B�u���[
static const int NO_ASMBL = -1;

// ���ꕶ���錾
static const char TAB = 0x09;
static const char SPC = 0x20;
static const int Fn = 0xf0;   // �K��
static const int PRTSC = 0xce;  // print screen?

// �L�[�z��̍ő吔
#define ROWMAX  5
#define COLMAX  8
static const int SUM = 40;

// �����V���b�g�����������[�v�� 
// ���̃��[�v�����o�߂���Ǝ����I�Ƀ����V���b�g�̓L�����Z������܂�
// ���o�I��10�J�E���g��0.3�b�ȉ��B
// �������ė����܂łɂق�̈�ċz����ƃ����V���b�g�͓����Ȃ������B
// �t�Ƀ����V���b�g�𔭓������悤�ƃL�[���������Ă����ɗ������ꍇ�ŁA���͂ɂ������鎖�͂قږ����B
// �C���L�[�Ƃ��ĉ������ĉ������̃L�[��c�������ۂ̌딚���S���N���Ȃ��Ȃ����B
static const int ACTIVE_LOOP = 10;
