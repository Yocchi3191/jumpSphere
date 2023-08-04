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
	//カメラを2台設定 
	ofCamera cam[2];
	ofLight light;
	ofImage imgFloor, imgBall;
	ofTexture texFloor, texBall;
};

//床のクラス
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
		//半透明の床面の描画（半透明な物体は最後に描画）
		ofPushMatrix();
		ofSetColor(ofFloatColor(0.7, 0.7, 0.7, 0.6));
		ofRotateDeg(-90, 1.0, 0.0, 0.0);
		tex.draw(pos.x, pos.y, pos.z, width, height);
		//Tex：左下座標(x,y,z)と幅(w)，高さ(h)を与える 
		ofPopMatrix();
	}
};

//球のクラス
class Ball {
public:
	float speed; //移動速度
	float gravity; //重力
	float vel; //ジャンプ力
	ofVec3f curPos; //現在の座標
	ofVec3f r; //現在の回転度合い

	int time; //ジャンプ処理で使用
	bool canJump; //ジャンプ判定
	ofVec3f addr; //回転量
	ofVec3f prePos; //直前の座標
	Floor currentFloor; //設置している床の情報を格納


	//デフォルトコンストラクタ
	Ball() {
		speed = 30.0f;
		vel = 3.0f;
		gravity = 0.3f;
		curPos = ofVec3f(0.0f, 0.0f, 0.0f);
		r = ofVec3f(0, -90, 0);
		canJump = true;
	}

	//メンバ変数のセット
	//まとめ版
	void set(float s, float jP, ofVec3f cP, ofVec3f setr) {
		setSpeed(s);
		setJumpPow(jP);
		setCurPos(cP);
		setR(setr);
	}
	//個別設定用
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

	//常に行う処理
	// ofApp update()内で計算させたい処理はここに
	void update() {
		updatePos();
	}

	//ボールの描画
	//drawで使用
	void draw(ofTexture tex) {
		//移動する球の描画
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

	//映り込みの描画
	//drawで使用
	void drawRefrection(ofTexture tex) {
		// 映り込む球の描画
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

	//キー押下による移動
	//keyPressedで使用
	void getKeyPush(int key) {
		keyMove(key);
	}

	//キーを離したときの処理
	void getKeyRelease(int key) {
		if (key == OF_KEY_UP || key == OF_KEY_DOWN) {
			addr.z = 0.0f;
		}
		if (key == OF_KEY_RIGHT || key == OF_KEY_LEFT) {
			addr.y = 0.0f;
		}
	}

	//マウスドラッグによる移動
	//mouseDraggedで使用
	void getMouseDrag(int x, int y, int button) {
		//ボールの回転角の設定  
		addr.y = float(x - ofGetWindowWidth() / 2) / 80;
		addr.z = float(y - ofGetWindowHeight() / 2) / 10;
	}

	//マウスボタンを離したときの処理
	void getMouseRelease(int x, int y, int button) {
		addr = ofVec3f(0, 0, 0);
	}

private:
	//方向角，移動量の計算
	void updatePos() {
		//ボールの回転
		r += addr;  //回転角を加える 
		float d = -2.0 * PI * addr.z / 360;  //移動距離の計算
		curPos.x = d * cos((r.y) / 180 * PI) + prePos.x;
		curPos.z = d * sin((r.y) / 180 * PI) + prePos.z;
		//ジャンプ処理
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

	//上下左右キー入力の取得, 対応する方向への回転量の設定
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

	//ジャンプ
	void jump() {
		if (canJump) {
			canJump = false;
			time = 0;
		}
	}
};