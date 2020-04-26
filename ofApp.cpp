#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 12 == 0) {

		ofMesh mesh, frame;
		frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
		int r = ofRandom(3);
		switch (r) {
		case 0:
			mesh.addVertex(glm::vec3(10, 0, 0));
			mesh.addVertex(glm::vec3(-10, 0, 0));
			frame.addVertex(glm::vec3(10, 0, 0));
			frame.addVertex(glm::vec3(-10, 0, 0));
			break;
		case 1:
			mesh.addVertex(glm::vec3(0, 10, 0));
			mesh.addVertex(glm::vec3(0, -10, 0));
			frame.addVertex(glm::vec3(0, 10, 0));
			frame.addVertex(glm::vec3(0, -10, 0));
			break;
		case 2:
			mesh.addVertex(glm::vec3(0, 0, 10));
			mesh.addVertex(glm::vec3(0, 0, -10));
			frame.addVertex(glm::vec3(0, 0, 10));
			frame.addVertex(glm::vec3(0, 0, -10));
			break;
		}
		this->mesh_list.push_back(mesh);
		this->frame_list.push_back(frame);

		auto direction = glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
		direction = glm::normalize(direction) * 30;
		this->direction_list.push_back(direction);

		auto noise_seed = glm::vec3(glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000)));
		this->noise_seed_list.push_back(noise_seed);

		this->color_list.push_back(ofColor(239, 39, 39, 192));
	}

	for (int i = this->mesh_list.size() - 1; i > -1; i--) {

		auto angle_x = ofMap(ofNoise(this->noise_seed_list[i].x, ofGetFrameNum() * 0.01), 0, 1, -PI, PI);
		auto angle_y = ofMap(ofNoise(this->noise_seed_list[i].y, ofGetFrameNum() * 0.01), 0, 1, -PI, PI);
		auto angle_z = ofMap(ofNoise(this->noise_seed_list[i].z, ofGetFrameNum() * 0.01), 0, 1, -PI, PI);

		auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
		auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
		auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

		glm::vec3 rotate_direction = glm::vec4(this->direction_list[i], 0) * rotation_z * rotation_y * rotation_x;

		auto right = this->mesh_list[i].getVertex(this->mesh_list[i].getNumVertices() - 2);
		auto left = this->mesh_list[i].getVertex(this->mesh_list[i].getNumVertices() - 1);

		this->mesh_list[i].addVertex(right + rotate_direction);
		this->mesh_list[i].addVertex(left + rotate_direction);

		this->frame_list[i].addVertex(right + rotate_direction);
		this->frame_list[i].addVertex(left + rotate_direction);

		int index = this->mesh_list[i].getNumVertices() - 1;
		this->mesh_list[i].addIndex(index); this->mesh_list[i].addIndex(index - 2); this->mesh_list[i].addIndex(index - 3);
		this->mesh_list[i].addIndex(index); this->mesh_list[i].addIndex(index - 1); this->mesh_list[i].addIndex(index - 3);

		this->frame_list[i].addIndex(index); this->frame_list[i].addIndex(index - 2);
		this->frame_list[i].addIndex(index - 1); this->frame_list[i].addIndex(index - 3);

		auto num = this->mesh_list[i].getNumVertices();
		this->color_list[i] = num < 300 ? this->color_list[i] : ofColor(this->color_list[i], ofMap(num, 300, 450, 192, 0));
		if (num > 450) {

			this->mesh_list.erase(this->mesh_list.begin() + i);
			this->frame_list.erase(this->frame_list.begin() + i);
			this->direction_list.erase(this->direction_list.begin() + i);
			this->noise_seed_list.erase(this->noise_seed_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofSetColor(239, 39, 39);
	ofDrawSphere(80);

	for (int i = 0; i < this->mesh_list.size(); i++) {

		ofSetColor(this->color_list[i]);
		this->mesh_list[i].draw();

		ofSetColor(239, 39, 39, this->color_list[i].a * 2);
		this->frame_list[i].drawWireframe();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}