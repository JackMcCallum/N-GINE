#include "stdafx.h"
#include "ViewPort.h"
#include <glm/ext.hpp>
#include "ObjFile.h"

ViewPort::ViewPort(QWidget *parent)
	: QGLWidget(parent)
{
	QGLFormat format = this->format();
	format.setSamples(8);
	this->setFormat(format);

	connect(&mRealtimeTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
	mRealtimeTimer.setInterval(16);
	mRealtimeTimer.start();

	mCamCurrent.x = 45;
	mCamCurrent.y = 20;
	mCamCurrent.z = 3;

	mDisplayModel = 0;
	mDisplayVerts = 0;
	mBackgroundModel = 0;
	mBackgroundVerts = 0;

	for (int i = 0; i < 6; i++)
		mScaledOutOfDate[i] = false;
	mIsInitialized = false;

	mColor = Qt::white;
}

ViewPort::~ViewPort()
{
	if (mDisplayModel)
		glDeleteBuffers(1, &mDisplayModel);

	if (mRealtimeTextureHandle)
		glDeleteTextures(1, &mRealtimeTextureHandle);

}

void ViewPort::initializeGL()
{
	initializeGLFunctions();
	mIsInitialized = true;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	loadShaders();

	updateMicrosurface(600);
	updateReflectivity(10000);
	updateFresnel(10000);
	updateSampleCount(2000);

	// Setup the texture
	glActiveTexture(0);
	glGenTextures(1, &mRealtimeTextureHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mRealtimeTextureHandle);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	chooseSide("../Data/Test/TropicalSunnyDayLeft2048.png", POS_X);
	chooseSide("../Data/Test/TropicalSunnyDayRight2048.png", NEG_X);
	chooseSide("../Data/Test/TropicalSunnyDayUp2048.png", POS_Y);
	chooseSide("../Data/Test/TropicalSunnyDayDown2048.png", NEG_Y);
	chooseSide("../Data/Test/TropicalSunnyDayFront2048.png", POS_Z);
	chooseSide("../Data/Test/TropicalSunnyDayBack2048.png", NEG_Z);
	

	// Load the models
	mDisplayVerts = loadModel(&mDisplayModel, "../Data/Test/N-Sphere.obj");
	mBackgroundVerts = loadModel(&mBackgroundModel, "../Data/Test/IcoSphere.obj");

}

void ViewPort::resizeGL(int w, int h)
{
	mProjMatrix = glm::perspective(50.0f, (float)w / (float)h, 0.1f, 1024.0f);
	glViewport(0, 0, w, h);
}

void ViewPort::paintGL()
{
	if (!QApplication::activeWindow())
		return;

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	updateCamera();

	unsigned int loc;
	glBindTexture(GL_TEXTURE_CUBE_MAP, mRealtimeTextureHandle);

	glm::mat4 projViewMat = mProjMatrix * mViewMatrix;
	mRealtimeFilterShader.bind();
	loc = glGetUniformLocation(mRealtimeFilterShader.programId(), "uCameraMatrix");
	if (loc != -1) glUniformMatrix4fv(loc, 1, false, (float*)&mCamMatrix);
	loc = glGetUniformLocation(mRealtimeFilterShader.programId(), "uProjViewModel");
	if (loc != -1) glUniformMatrix4fv(loc, 1, false, (float*)&projViewMat);
	glBindBuffer(GL_ARRAY_BUFFER, mDisplayModel);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(ObjFile::Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(ObjFile::Vertex), (char*)0 + sizeof(float)* 3);
	glDrawArrays(GL_TRIANGLES, 0, mDisplayVerts);

	mBackgroundShader.bind();
	loc = glGetUniformLocation(mBackgroundShader.programId(), "uCameraMatrix");
	if (loc != -1) glUniformMatrix4fv(loc, 1, false, (float*)&mCamMatrix);
	loc = glGetUniformLocation(mBackgroundShader.programId(), "uProjViewModel");
	if (loc != -1) glUniformMatrix4fv(loc, 1, false, (float*)&(projViewMat * glm::mat4(512, 0, 0, 0, 0, 512, 0, 0, 0, 0, 512, 0, 0, 0, 0, 1)));
	glBindBuffer(GL_ARRAY_BUFFER, mBackgroundModel);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(ObjFile::Vertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(ObjFile::Vertex), (char*)0 + sizeof(float)* 3);
	glDrawArrays(GL_TRIANGLES, 0, mBackgroundVerts);

}

void ViewPort::mousePressEvent(QMouseEvent* evt)
{
	mIsDragging = true;
	mLastMousePos.x = evt->x();
	mLastMousePos.y = evt->y();
}

void ViewPort::mouseReleaseEvent(QMouseEvent* evt)
{
	mIsDragging = false;
}

void ViewPort::mouseMoveEvent(QMouseEvent* evt)
{
	mCamAcceleration.x += (mLastMousePos.x - evt->x()) * 0.2f;
	mCamAcceleration.y -= (mLastMousePos.y - evt->y()) * 0.2f;

	mLastMousePos.x = evt->x();
	mLastMousePos.y = evt->y();
}

void ViewPort::wheelEvent(QWheelEvent* evt)
{
	mCamAcceleration.z -= evt->delta() * 0.005f;
}

void ViewPort::updateCamera()
{
	const float pi = glm::pi<float>();

	mCamCurrent += mCamAcceleration;
	mCamAcceleration *= 0.5f;

	if (mCamCurrent.x > 360)
		mCamCurrent.x -= 360;
	if (mCamCurrent.x < 360)
		mCamCurrent.x += 360;

	if (mCamCurrent.y > 89)
		mCamCurrent.y = 89;
	if (mCamCurrent.y < -89)
		mCamCurrent.y = -89;

	if (mCamCurrent.z < 1)
		mCamCurrent.z = 1;

	glm::vec3 position;
	position.x = glm::sin(mCamCurrent.x * pi / 180) * glm::cos(mCamCurrent.y * pi / 180);
	position.y = glm::sin(mCamCurrent.y * pi / 180);
	position.z = glm::cos(mCamCurrent.x * pi / 180) * glm::cos(mCamCurrent.y * pi / 180);
	position *= mCamCurrent.z;

	glm::vec3 lookat(0.0f);
	glm::vec3 up(0, 1, 0);

	glm::vec3 xAxis, yAxis, zAxis;
	zAxis = glm::normalize(position - lookat);
	xAxis = glm::normalize(glm::cross(up, zAxis));
	yAxis = glm::cross(zAxis, xAxis);

	mCamMatrix[0] = glm::vec4(xAxis, 0);
	mCamMatrix[1] = glm::vec4(yAxis, 0);
	mCamMatrix[2] = glm::vec4(zAxis, 0);
	mCamMatrix[3] = glm::vec4(position, 1);
	mViewMatrix = glm::inverse(mCamMatrix);
}

void ViewPort::chooseSide(Side side)
{
	QString filename = QFileDialog::getOpenFileName(this, "Open File");
	if (filename.isEmpty())
		return;

	chooseSide(filename.toStdString(), side);
}

void ViewPort::chooseSide(const std::string& fname, Side side)
{
	makeCurrent();

	// Load it into a temp image, in case it fails it wont screw with our old data
	QImage tmpImage;
	if (!tmpImage.load(fname.c_str()))
		return;

	// Success, move the new image over the old data
	mOriginalImages[side] = tmpImage;

	QFileInfo fileInfo(fname.c_str());
	mSelectedImageLabels[side]->setText(fileInfo.fileName());
	mSelectedImageLabels[side]->setToolTip(fileInfo.fileName());

	mScaledOutOfDate[side] = true;
	uploadCubeMap();
}

void ViewPort::texSizeChanged(QString value)
{
	unsigned int s = 1;
	if (value == "1x1") s = 1;
	else if (value == "2x2") s = 2;
	else if (value == "4x4") s = 4;
	else if (value == "8x8") s = 8;
	else if (value == "16x16") s = 16;
	else if (value == "32x32") s = 32;
	else if (value == "64x64") s = 64;
	else if (value == "128x128") s = 128;
	else if (value == "256x256") s = 256;
	else if (value == "512x512") s = 512;
	else if (value == "1024x1024") s = 1024;
	else if (value == "2048x2048") s = 2048;
	else { assert(false); }
	mTopLevelMipMapSize = s;

	for (int i = 0; i < 6; i++)
		mScaledOutOfDate[i] = true;

	if (mIsInitialized)
		uploadCubeMap();

}






void ViewPort::openDisplayModel()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open File", "", "*.obj");
	if (filename.isEmpty())
		return;

	mDisplayVerts = loadModel(&mDisplayModel, filename.toStdString());
}

unsigned int ViewPort::loadModel(unsigned int* handle, const std::string& fname)
{
	makeCurrent();

	unsigned int h = *handle;

	if (!h)
		glGenBuffers(1, &h);

	ObjFile file;
	if (!file.load(fname))
		return 0;

	glBindBuffer(GL_ARRAY_BUFFER, h);
	glBufferData(GL_ARRAY_BUFFER, file.size, file.data, GL_STATIC_DRAW);

	*handle = h;
	return file.size / sizeof(ObjFile::Vertex);

	doneCurrent();
}

void ViewPort::loadShaders()
{
	unsigned int loc;
	// Setup the shaders
	mRealtimeFilterShader.addShaderFromSourceFile(QGLShader::Vertex, "../Data/Test/CubeMapShader.vert");
	mRealtimeFilterShader.addShaderFromSourceFile(QGLShader::Fragment, "../Data/Test/CubeMapShader.frag");
	mRealtimeFilterShader.link();

	mBackgroundShader.addShaderFromSourceFile(QGLShader::Vertex, "../Data/Test/BackgroundShader.vert");
	mBackgroundShader.addShaderFromSourceFile(QGLShader::Fragment, "../Data/Test/BackgroundShader.frag");
	mBackgroundShader.link();

	loc = glGetUniformLocation(mRealtimeFilterShader.programId(), "uCubeMap");
	if (loc != -1) glUniform1i(loc, 0);

	loc = glGetUniformLocation(mBackgroundShader.programId(), "uCubeMap");
	if (loc != -1) glUniform1i(loc, 0);
}

void ViewPort::uploadCubeMap()
{
	size_t sizeInBytes = mTopLevelMipMapSize * mTopLevelMipMapSize * 3;
	unsigned char* imageData = new unsigned char[sizeInBytes];

	for (int i = 0; i < 6; i++)
	{
		if (mScaledOutOfDate[i] && !mOriginalImages[i].isNull())
		{
			mScaledOutOfDate[i] = false;
			mScaledImages[i] = mOriginalImages[i].scaled(
				mTopLevelMipMapSize, mTopLevelMipMapSize, 
				Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

			// Extract image data from scale image
			for (int x = 0; x < mTopLevelMipMapSize; x++)
			{
				for (int y = 0; y < mTopLevelMipMapSize; y++)
				{
					union
					{
						struct { char b, g, r, a; } separate;
						QRgb qrgb;
					} col;
					col.qrgb = mScaledImages[i].pixel(x, y);

					imageData[y * 3 * mTopLevelMipMapSize + x * 3 + 0] = col.separate.r;
					imageData[y * 3 * mTopLevelMipMapSize + x * 3 + 1] = col.separate.g;
					imageData[y * 3 * mTopLevelMipMapSize + x * 3 + 2] = col.separate.b;
				}
			}

			// Give the data to OpenGL
			glBindTexture(GL_TEXTURE_CUBE_MAP, mRealtimeTextureHandle);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				mTopLevelMipMapSize, mTopLevelMipMapSize,
				0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		}
		else if (mOriginalImages[i].isNull())
		{
			memset(imageData, 0, sizeInBytes);
			glBindTexture(GL_TEXTURE_CUBE_MAP, mRealtimeTextureHandle);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				mTopLevelMipMapSize, mTopLevelMipMapSize, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
	}

	delete[] imageData;
}

void ViewPort::updateMicrosurface(int val)
{
	mRealtimeFilterShader.bind();
	float fval = (val / 10000.0f);
	unsigned int loc = glGetUniformLocation(mRealtimeFilterShader.programId(), "uMicrosurface");
	if (loc != -1) glUniform1f(loc, fval);
}

void ViewPort::updateReflectivity(int val)
{
	mRealtimeFilterShader.bind();
	float fval = (val / 10000.0f);
	unsigned int loc = glGetUniformLocation(mRealtimeFilterShader.programId(), "uReflectivity");
	if (loc != -1) glUniform1f(loc, fval);
}

void ViewPort::updateFresnel(int val)
{
	mRealtimeFilterShader.bind();
	float fval = (val / 10000.0f);
	unsigned int loc = glGetUniformLocation(mRealtimeFilterShader.programId(), "uFresnel");
	if (loc != -1) glUniform1f(loc, fval);
}

void ViewPort::updateDisplayModelScale(int val)
{

}

void ViewPort::updateSampleCount(int val)
{
	float fval = (1 - glm::pow(1.0f - (val / 10000.0f), 2.0f)) * 200.0f;
	int aval = (int)glm::max(1.0f, fval);

	mRealtimeFilterShader.bind();
	unsigned int loc = glGetUniformLocation(mRealtimeFilterShader.programId(), "uSampleCount");
	if (loc != -1) glUniform1i(loc, aval);
}

void ViewPort::chooseColor()
{
	QColor color = QColorDialog::getColor(mColor, this);
	if (color.isValid())
	{
		mRealtimeFilterShader.bind();
		unsigned int loc = glGetUniformLocation(mRealtimeFilterShader.programId(), "uAlbedo");
		if (loc != -1) glUniform3f(loc, color.redF(), color.greenF(), color.blueF());
		mColor = color;
	}
}


