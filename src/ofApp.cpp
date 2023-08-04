#include "ofApp.h"

Ball ball;
vector<Floor> floors;

ofVec3f lightPos;
ofVec2f preMousePos;
int camId;
//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(30);

	//2�̃J�����̏����ݒ�  
	cam[0].setNearClip(0.5);
	cam[0].setFarClip(1000);
	cam[0].setFov(60);
	cam[0].setPosition(0, 3.0, 10);
	cam[1].setNearClip(0.5);
	cam[1].setFarClip(1000);
	cam[1].setFov(60);
	cam[1].setPosition(0, 100, 0);
	cam[1].setOrientation(ofVec3f(-90, 0, 0));

	light.enable();  //light��L����  

	//�e�N�X�`���}�b�s���O�̐ݒ�
	ofDisableArbTex();

	//tex1  
	imgFloor.load("check.jpg");  //�摜�t�@�C����bin�̉���data�t�H���_�ɓ���� 
	//RGB��3�`�����l���摜
	texFloor.loadData(imgFloor);
	texFloor.setTextureWrap(GL_REPEAT, GL_REPEAT);  //�c���Ƃ��J��Ԃ�
	texFloor.setTextureMinMagFilter(GL_NEAREST, GL_LINEAR); //��ԕ��@����`��Ԃ�
	texFloor.texData.tex_t = 50; //���̌J��Ԃ��� 
	texFloor.texData.tex_u = 50; //�c�̌J��Ԃ���  

	//���ɓ\��摜�̐ݒ�
	imgBall.load("ball.jpg"); //�摜�t�@�C����bin�̉���data�t�H���_�ɓ����
	imgBall.mirror(false, true);
	texBall.loadData(imgBall);

	//���̐ݒ�
	floors.push_back(Floor(ofVec3f(-25, 0, 0), 50, 50)); //��{��
	//�����_����
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
	//���̐ݒ� 
	light.setAmbientColor((ofFloatColor(0.6, 0.6, 0.6, 1)));
	light.setDiffuseColor((ofFloatColor(0.6, 0.6, 0.6, 1)));
	light.setSpecularColor((ofFloatColor(0.6, 0.6, 0.6, 1)));


	// �{�[�������̏�ɂ��邩�ǂ����𔻒肷��
	bool isOnFloor = false;
	for each (Floor floor in floors) {
		// �{�[���̍��W�����͈͓̔��ɂ���ꍇ�A���̏�ɂ���Ɣ���
		if (ball.curPos.x >= floor.pos.x - floor.width / 2 &&
			ball.curPos.x <= floor.pos.x + floor.width / 2 &&
			ball.curPos.z >= floor.pos.y - floor.height / 2 &&
			ball.curPos.z <= floor.pos.y + floor.height / 2 &&
			ball.currentFloor.pos.y >= floor.pos.z &&
			ball.currentFloor.pos.y <= floor.pos.z + 2
			) {
			isOnFloor = true;
			// �{�[�������̏�ɂ���ꍇ�AcurrentFloor���X�V����
			ball.currentFloor = floor;
		}
	}
	// �{�[���̋�������������
	if (isOnFloor) {
		// �{�[�������̏�ɂ���ꍇ�A���̏�Ɉʒu���Œ肷��
		ball.curPos.y = ball.currentFloor.pos.y;
		ball.canJump = true; // �W�����v��������Z�b�g
	}

	ball.update(); // �{�[���̈ʒu���X�V

	cout << "ball pos :" << ball.curPos << endl;
	cout << "current floor  :" << ball.currentFloor.pos << endl;
}

//--------------------------------------------------------------
void ofApp::draw() {
	cam[camId].begin();
	ofEnableDepthTest();  //�[�������L����  
	ofEnableSmoothing();  //�X���[�W���O������L���� 
	ofEnableLighting();  //Lighting��L���� 
	ofBackground(ofFloatColor(0.3, 0.3, 0.4));


	ofPushMatrix();
	lightPos.set(2.0, 0.0, 2.0);
	light.setPosition(lightPos);
	light.setPointLight();

	//�{�[���̕`��
	ball.draw(texBall);

	//���̕`��
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
	//�{�^���������ꂽ��ݒ�l�𔽓]���� 
	//�J����ID��؂�ւ���  
	camId = !camId;
}