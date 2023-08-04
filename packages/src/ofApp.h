#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseReleased(int x, int y, int button);

	void btnCameraPressed();
	//�J������2��ݒ� 
	ofCamera cam[2];
	ofLight light;
	ofImage imgFloor, imgBall;
	ofTexture texFloor, texBall;
};

//���̃N���X
class Floor {
public:
	ofVec3f pos;
	int width;
	int height;

	Floor() {
		pos = ofVec3f(0, 0, 0);
		width = 50;
		height = 50;
	}
	Floor(ofVec3f setPos, int setWidth, int setHeight)
		:pos(setPos), width(setWidth), height(setHeight)
	{
	}

	void draw(ofTexture tex) {
		//�������̏��ʂ̕`��i�������ȕ��͍̂Ō�ɕ`��j
		ofPushMatrix();
		ofSetColor(ofFloatColor(0.7, 0.7, 0.7, 0.6));
		ofRotateDeg(-90, 1.0, 0.0, 0.0);
		tex.draw(pos.x, pos.y, pos.z, width, height);
		//Tex�F�������W(x,y,z)�ƕ�(w)�C����(h)��^���� 
		ofPopMatrix();
	}
};

//���̃N���X
class Ball {
public:
	float speed; //�ړ����x
	float gravity; //�d��
	float vel; //�W�����v��
	ofVec3f curPos; //���݂̍��W
	ofVec3f r; //���݂̉�]�x����

	int time; //�W�����v�����Ŏg�p
	bool canJump; //�W�����v����
	ofVec3f addr; //��]��
	ofVec3f prePos; //���O�̍��W
	Floor currentFloor; //�ݒu���Ă��鏰�̏����i�[


	//�f�t�H���g�R���X�g���N�^
	Ball() {
		speed = 30.0f;
		vel = 3.0f;
		gravity = 0.3f;
		curPos = ofVec3f(0.0f, 0.0f, 0.0f);
		r = ofVec3f(0, -90, 0);
		canJump = true;
	}

	//�����o�ϐ��̃Z�b�g
	//�܂Ƃߔ�
	void set(float s, float jP, ofVec3f cP, ofVec3f setr) {
		setSpeed(s);
		setJumpPow(jP);
		setCurPos(cP);
		setR(setr);
	}
	//�ʐݒ�p
	void setSpeed(float s) {
		speed = s;
	}
	void setJumpPow(float jP) {
		vel = jP;
	}
	void setGravity(float g) {
		gravity = g;
	}
	void setCurPos(ofVec3f cP) {
		curPos = cP;
	}
	void setR(ofVec3f setr) {
		r = setr;
	}

	//��ɍs������
	// ofApp update()���Ōv�Z�������������͂�����
	void update() {
		updatePos();
	}

	//�{�[���̕`��
	//draw�Ŏg�p
	void draw(ofTexture tex) {
		//�ړ����鋅�̕`��
		ofPushMatrix();
		ofSetColor(ofFloatColor(0.7, 0.7, 0.7, 1.0));
		ofTranslate(0.0, 0.90, 0.0);
		ofTranslate(curPos);
		ofRotateDeg(-r.y, 0.0, 1.0, 0.0);
		ofRotateDeg(r.z, 0.0, 0.0, 1.0);
		tex.bind();
		ofDrawSphere(1.0);
		tex.unbind();
		ofPopMatrix();
	}

	//�f�荞�݂̕`��
	//draw�Ŏg�p
	void drawRefrection(ofTexture tex) {
		// �f�荞�ދ��̕`��
		ofPushMatrix();
		ofScale(1.0, -1.0, 1.0);
		ofSetColor(ofFloatColor(0.7, 0.7, 0.7, 1.0));
		ofTranslate(0.0, 0.90, 0.0);
		ofTranslate(curPos.x, curPos.y, curPos.z);
		ofRotateDeg(-r.y, 0.0, 1.0, 0.0);
		ofRotateDeg(r.z, 0.0, 0.0, 1.0);
		tex.bind();
		ofDrawSphere(1.0);
		tex.unbind();
		ofPopMatrix();
	}

	//�L�[�����ɂ��ړ�
	//keyPressed�Ŏg�p
	void getKeyPush(int key) {
		keyMove(key);
	}

	//�L�[�𗣂����Ƃ��̏���
	void getKeyRelease(int key) {
		if (key == OF_KEY_UP || key == OF_KEY_DOWN) {
			addr.z = 0.0f;
		}
		if (key == OF_KEY_RIGHT || key == OF_KEY_LEFT) {
			addr.y = 0.0f;
		}
	}

	//�}�E�X�h���b�O�ɂ��ړ�
	//mouseDragged�Ŏg�p
	void getMouseDrag(int x, int y, int button) {
		//�{�[���̉�]�p�̐ݒ�  
		addr.y = float(x - ofGetWindowWidth() / 2) / 80;
		addr.z = float(y - ofGetWindowHeight() / 2) / 10;
	}

	//�}�E�X�{�^���𗣂����Ƃ��̏���
	void getMouseRelease(int x, int y, int button) {
		addr = ofVec3f(0, 0, 0);
	}

private:
	//�����p�C�ړ��ʂ̌v�Z
	void updatePos() {
		//�{�[���̉�]
		r += addr;  //��]�p�������� 
		float d = -2.0 * PI * addr.z / 360;  //�ړ������̌v�Z
		curPos.x = d * cos((r.y) / 180 * PI) + prePos.x;
		curPos.z = d * sin((r.y) / 180 * PI) + prePos.z;
		//�W�����v����
		if (canJump) {
			prePos = curPos;
			return;
		}
		float ground = currentFloor.pos.z;
		curPos.y = -(0.5 * gravity * time * time - vel * time + ground);
		if (curPos.y < ground) {
			curPos.y = ground;
			canJump = true;
		}
		time += 1;
		prePos = curPos;
	}

	//�㉺���E�L�[���͂̎擾, �Ή���������ւ̉�]�ʂ̐ݒ�
	void keyMove(int key) {
		if (key == OF_KEY_UP) {
			addr.z = float(-speed);
		}
		if (key == OF_KEY_DOWN) {
			addr.z = float(speed);
		}
		if (key == OF_KEY_RIGHT) {
			addr.y = float(speed / 5);
		}
		if (key == OF_KEY_LEFT) {
			addr.y = float(-speed / 5);
		}
		if (key == ' ') {
			jump();
		}
	}

	//�W�����v
	void jump() {
		if (canJump) {
			canJump = false;
			time = 0;
		}
	}
};