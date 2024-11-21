#include "OpenGlWidget.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>

using namespace std;

OpenGlWidget::OpenGlWidget(QWidget* parent)
	: QOpenGLWidget(parent),
	isInitialized(false)
{
}

OpenGlWidget::~OpenGlWidget() {
	makeCurrent();
	for (DrawingObject drawingObject : drawingObjects)
	{
		//drawingObject.vao->destroy();
		//drawingObject.vbo.destroy();
	}
	doneCurrent();
}

void OpenGlWidget::setData(QVector<Data> inData)
{
	if (inData.size() > 0)
	{
		data = inData;
		makeCurrent();
		initializeGL();
		update();
	}
}

QSize OpenGlWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize OpenGlWidget::sizeHint() const
{
	return QSize(800, 800);
}

void OpenGlWidget::sync(float inZoomLevel, QVector3D inRotation, QVector2D inPanOffset)
{
	zoomLevel = inZoomLevel;
	rotation = inRotation;
	panOffset = inPanOffset;
	update();
}

void OpenGlWidget::initializeGL()
{
	if (!isInitialized && data.size() > 0)
	{
		initializeOpenGLFunctions();
		glEnable(GL_DEPTH_TEST);
		createArrayAndBuffers();
	}
}


void OpenGlWidget::resizeGL(int w, int h) {
	if (isInitialized == true)
	{
		glViewport(0, 0, w, h);
	}
	projection.setToIdentity();
	projection.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);
}

void OpenGlWidget::paintGL()
{
	if (isInitialized == true)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		updateModelViewMatrix();

		for (DrawingObject drawingObject : drawingObjects)
		{
			if (drawingObject.drawStyle == DrawStyle::TRIANGLES)
			{
				shaderProgram.bind();
				shaderProgram.setUniformValue("projection", projection);
				shaderProgram.setUniformValue("modelView", modelView);

				QVector3D lightPos(0.5f, 0.5f, 10.0f);
				shaderProgram.setUniformValue("lightPos", lightPos);
				shaderProgram.setUniformValue("viewPos", QVector3D(0.0f, 0.0f, 5.0f));

				drawingObject.vao->bind();
				glDrawArrays(GL_TRIANGLES, 0, drawingObject.numVertices);
				drawingObject.vao->release();

				shaderProgram.release();
			}
			else if (drawingObject.drawStyle == DrawStyle::LINES)
			{
				shaderProgram1.bind();
				shaderProgram1.setUniformValue("projection", projection);
				shaderProgram1.setUniformValue("modelView", modelView);

				QVector3D lightPos(0.5f, 0.5f, 10.0f);
				shaderProgram1.setUniformValue("lightPos", lightPos);
				shaderProgram1.setUniformValue("viewPos", QVector3D(0.0f, 0.0f, 5.0f));

				drawingObject.vao->bind();
				glDrawArrays(GL_LINES, 0, drawingObject.numVertices);
				drawingObject.vao->release();

				shaderProgram1.release();
			}
		}
	}
}

void OpenGlWidget::updateModelViewMatrix() {
	modelView.setToIdentity();
	modelView.translate(panOffset.x(), panOffset.y(), -5.0f);
	modelView.scale(zoomLevel);
	modelView.rotate(rotation.x(), 1.0f, 0.0f, 0.0f);
	modelView.rotate(rotation.y(), 0.0f, 1.0f, 0.0f);
	modelView.rotate(rotation.z(), 0.0f, 0.0f, 1.0f);
}

void OpenGlWidget::createArrayAndBuffers()
{
	// Load shader program
	bool shaderLoaded = false;

	shaderLoaded = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/shadedVert.glsl");
	shaderLoaded = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/shadedFrag.glsl");
	shaderProgram.link();

	shaderLoaded = shaderProgram1.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/wireVert.glsl");
	shaderLoaded = shaderProgram1.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/wireFrag.glsl");
	shaderProgram1.link();

	for (Data dt : data)
	{
		//if (dt.vertices.size() == dt.normals.size())
		{
			DrawingObject drawingObject;
			if (dt.drawStyle == DrawStyle::TRIANGLES)
			{
				std::vector<float> vertexData;
				for (int i = 0; i < dt.vertices.size(); i = i + 3)
				{
					vertexData.push_back(dt.vertices[i]);
					vertexData.push_back(dt.vertices[i + 1]);
					vertexData.push_back(dt.vertices[i + 2]);
					vertexData.push_back(dt.normals[i]);
					vertexData.push_back(dt.normals[i + 1]);
					vertexData.push_back(dt.normals[i + 2]);
					if (dt.colors.size() == dt.vertices.size())
					{
						vertexData.push_back(dt.colors[i]);
						vertexData.push_back(dt.colors[i + 1]);
						vertexData.push_back(dt.colors[i + 2]);
					}
				}
				drawingObject.vao = new QOpenGLVertexArrayObject();
				drawingObject.vao->create();
				drawingObject.vao->bind();

				drawingObject.vbo.create();
				drawingObject.vbo.bind();
				drawingObject.vbo.allocate(vertexData.data(), static_cast<int>(vertexData.size() * sizeof(float)));

				shaderProgram.bind();
				shaderProgram.enableAttributeArray(0); // Position attribute
				shaderProgram.enableAttributeArray(1); // Normal attribute
				shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 9 * sizeof(float));
				shaderProgram.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 9 * sizeof(float));

				if (dt.colors.size() == dt.vertices.size())
				{
					shaderProgram.enableAttributeArray(2); // color attribute
					shaderProgram.setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(float), 3, 9 * sizeof(float));
				}

				shaderProgram.release();

				drawingObject.vbo.release();
			}
			else if (dt.drawStyle == DrawStyle::LINES)
			{
				std::vector<float> vertexData1;
				for (int i = 0; i < dt.vertices.size(); i = i + 3)
				{
					vertexData1.push_back(dt.vertices[i]);
					vertexData1.push_back(dt.vertices[i + 1]);
					vertexData1.push_back(dt.vertices[i + 2]);
					//vertexData1.push_back(dt.normals[i]);
					//vertexData1.push_back(dt.normals[i + 1]);
					//vertexData1.push_back(dt.normals[i + 2]);
					vertexData1.push_back(dt.colors[i]);
					vertexData1.push_back(dt.colors[i + 1]);
					vertexData1.push_back(dt.colors[i + 2]);
				}

				drawingObject.vao = new QOpenGLVertexArrayObject();
				drawingObject.vao->create();
				drawingObject.vao->bind();

				drawingObject.vbo.create();
				drawingObject.vbo.bind();
				drawingObject.vbo.allocate(vertexData1.data(), static_cast<int>(vertexData1.size() * sizeof(float)));
				glFlush();

				shaderProgram1.bind();
				shaderProgram1.enableAttributeArray(0); // Position attribute
				//shaderProgram1.enableAttributeArray(1); // Normal attribute
				shaderProgram1.enableAttributeArray(1); // color attribute
				shaderProgram1.setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
				shaderProgram1.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
				//shaderProgram1.setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(float), 3, 9 * sizeof(float));
				shaderProgram1.release();

				drawingObject.vbo.release();
			}

			drawingObject.numVertices = dt.vertices.size();
			drawingObject.drawStyle = dt.drawStyle;
			drawingObjects.push_back(drawingObject);
			isInitialized = true;
		}
	}
}

void OpenGlWidget::wheelEvent(QWheelEvent* event) {
	// Zoom in or out
	if (event->angleDelta().y() > 0)
		zoomLevel *= 1.1f;  // Zoom in
	else
		zoomLevel /= 1.1f;  // Zoom out
	update();
	viewChange(zoomLevel, rotation, panOffset);
}

void OpenGlWidget::mousePressEvent(QMouseEvent* event) {
	lastMousePosition = event->pos();
}

void OpenGlWidget::mouseMoveEvent(QMouseEvent* event) {
	QPoint delta = event->pos() - lastMousePosition;
	lastMousePosition = event->pos();

	if (event->buttons() & Qt::LeftButton) {
		// Rotate model
		rotation.setX(rotation.x() + delta.y() * 0.5f);
		rotation.setY(rotation.y() + delta.x() * 0.5f);
	}
	else if (event->buttons() & Qt::RightButton) {
		// Pan model
		panOffset.setX(panOffset.x() + delta.x() * 0.01f);
		panOffset.setY(panOffset.y() - delta.y() * 0.01f);
	}
	update();
	viewChange(zoomLevel, rotation, panOffset);
}
