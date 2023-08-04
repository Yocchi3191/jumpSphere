#include "ofApp.h"

Ball ball;
vector<Floor> floors;

ofVec3f lightPos;
ofVec2f preMousePos;
int camId;
//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(30);

	//2つのカメラの初期設定  
	cam[0].setNearClip(0.5);
	cam[0].setFarClip(1000);
	cam[0].setFov(60);
	cam[0].setPosition(0, 3.0, 10);
	cam[1].setNearClip(0.5);
	cam[1].setFarClip(1000);
	cam[1].setFov(60);
	cam[1].setPosition(0, 100, 0);
	cam[1].setOrientation(ofVec3f(-90, 0, 0));

	light.enable();  //lightを有効に  

	//テクスチャマッピングの設定
	ofDisableArbTex();

	//tex1  
	imgFloor.load("check.jpg");  //画像ファイルはbinの下のdataフォルダに入れる 
	//RGBの3チャンネル画像
	texFloor.loadData(imgFloor);
	texFloor.setTextureWrap(GL_REPEAT, GL_REPEAT);  //縦横とも繰り返し
	texFloor.setTextureMinMagFilter(GL_NEAREST, GL_LINEAR); //補間方法を線形補間に
	texFloor.texData.tex_t = 50; //横の繰り返し数 
	texFloor.texData.tex_u = 50; //縦の繰り返し数  

	//球に貼る画像の設定
	imgBall.load("ball.jpg"); //画像ファイルはbinの下のdataフォルダに入れる
	imgBall.mirror(false, true);
	texBall.loadData(imgBall);

	//床の設定
	floors.push_back(Floor(ofVec3f(-25, 0, 0), 50, 50)); //基本床
	//ランダム床
	for (int count = 0; count < 5; count++) {
		float x = 50 * ofRandomf();
		float y = 50 * ofRandomf();
		float z = 10 * (2 + ofRandomf());
		int width = ofRandom(10, 20);
		int height = ofRandom(10, 20);
		floors.push_back(Floor(ofVec3f(x, y, z), width, height));
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	//光の設定 
	light.setAmbientColor((ofFloatColor(0.6, 0.6, 0.6, 1)));
	light.setDiffuseColor((ofFloatColor(0.6, 0.6, 0.6, 1)));
	light.setSpecularColor((ofFloatColor(0.6, 0.6, 0.6, 1)));


	// ボールが床の上にいるかどうかを判定する
	bool isOnFloor = false;
	for each (Floor floor in floors) {
		// ボールの座標が床の範囲内にある場合、床の上にいると判定
		if (ball.curPos.x >= floor.pos.x - floor.width / 2 &&
			ball.curPos.x <= floor.pos.x + floor.width / 2 &&
			ball.curPos.z >= floor.pos.y - floor.height / 2 &&
			ball.curPos.z <= floor.pos.y + floor.height / 2 &&
			ball.currentFloor.pos.y >= floor.pos.z &&
			ball.currentFloor.pos.y <= floor.pos.z + 2
			) {
			isOnFloor = true;
			// ボールが床の上にいる場合、currentFloorを更新する
			ball.currentFloor = floor;
		}
	}
	// ボールの挙動を処理する
	if (isOnFloor) {
		// ボールが床の上にいる場合、床の上に位置を固定する
		ball.curPos.y = ball.currentFloor.pos.y;
		ball.canJump = true; // ジャンプ判定をリセット
	}

	ball.update(); // ボールの位置を更新

	cout << "ball pos :" << ball.curPos << endl;
	cout << "current floor  :" << ball.currentFloor.pos << endl;
}

//--------------------------------------------------------------
void ofApp::draw() {
	cam[camId].begin();
	ofEnableDepthTest();  //深さ判定を有効に  
	ofEnableSmoothing();  //スムージング処理を有効に 
	ofEnableLighting();  //Lightingを有効に 
	ofBackground(ofFloatColor(0.3, 0.3, 0.4));


	ofPushMatrix();
	lightPos.set(2.0, 0.0, 2.0);
	light.setPosition(lightPos);
	light.setPointLight();

	//ボールの描画
	ball.draw(texBall);

	//床の描画
	for each (Floor floor in floors)
	{
		floor.draw(texFloor);
	}

	ofVec3f	ballPos = ball.curPos;
	ofVec3f	ballR = ball.r;
	cam[camId].setPosition(ballPos.x + 15.0 * cos((ball.r.y + 180) / 180 * PI), ballPos.y + 5.0, ballPos.z + 15.0 * sin((ballR.y + 180) / 180 * PI));
	cam[camId].lookAt(ballPos);

	ofPopMatrix();
	ofDisableDepthTest();
	ofDisableSmoothing();
	ofDisableLighting();
	cam[camId].end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ball.getKeyPush(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	ball.getKeyRelease(key);
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	ball.getMouseRelease(x, y, button);
}

void ofApp::btnCameraPressed() {
	//ボタンが押されたら設定値を反転して 
	//カメラIDを切り替える  
	camId = !camId;
}