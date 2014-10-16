#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QGLWidget>
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <QtOpenGL/QGLShaderProgram>
#include <QtOpenGL/QGLFunctions>

class ViewPort : public QGLWidget, public QGLFunctions
{
	Q_OBJECT

public:
	ViewPort(QWidget *parent);
	~ViewPort();

	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();

	virtual void mousePressEvent(QMouseEvent* evt);
	virtual void mouseReleaseEvent(QMouseEvent* evt);
	virtual void mouseMoveEvent(QMouseEvent* evt);
	virtual void wheelEvent(QWheelEvent* evt);

	void updateCamera();
	void loadShaders();

	void uploadCubeMap();


	enum Side
	{
		POS_X,
		NEG_X,
		POS_Y,
		NEG_Y,
		POS_Z,
		NEG_Z,
	};

	void chooseSide(Side side);
	void chooseSide(const std::string& fname, Side side);
	unsigned int loadModel(unsigned int* handle, const std::string& fname);
	
	void renderTexture(unsigned int tex, unsigned int level);

public slots:

	void texSizeChanged(QString value);

	void openDisplayModel();

	void choosePosX() { chooseSide(POS_X); }
	void chooseNegX() { chooseSide(NEG_X); }
	void choosePosY() { chooseSide(POS_Y); }
	void chooseNegY() { chooseSide(NEG_Y); }
	void choosePosZ() { chooseSide(POS_Z); }
	void chooseNegZ() { chooseSide(NEG_Z); }

	void updateMicrosurface(int val);
	void updateReflectivity(int val);
	void updateFresnel(int val);
	void updateDisplayModelScale(int val);
	void updateSampleCount(int val);

	void chooseColor();

private:
	QTimer mRealtimeTimer;
	
	// Interaction & Camera
	glm::vec3 mCamCurrent;
	glm::vec3 mCamAcceleration;
	glm::ivec2 mLastMousePos;
	glm::mat4 mProjMatrix;
	glm::mat4 mViewMatrix;
	glm::mat4 mCamMatrix;
	bool mIsDragging;
	bool mIsInitialized;

	// GPU stuff
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
	};

	// Geometrys
	unsigned int mDisplayModel;
	unsigned int mDisplayVerts;
	unsigned int mBackgroundModel;
	unsigned int mBackgroundVerts;

	// Shaders
	QGLShaderProgram mRealtimeFilterShader;
	QGLShaderProgram mBackgroundShader;

	QColor mColor;
	unsigned int mTopLevelMipMapSize;

	unsigned int mRealtimeTextureHandle;
	unsigned int mFilteredTextureHandle;
	unsigned int mRenderTarget;

public:
	QLabel* mSelectedImageLabels[6];

	QImage mOriginalImages[6];
	QImage mScaledImages[6];
	bool mScaledOutOfDate[6];
};

#endif // VIEWPORT_H
