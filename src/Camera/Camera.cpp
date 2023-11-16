#include "Camera.h"
using namespace Eigen;

Camera::Camera():mViewIsUptodate(false), mProjIsUptodate(false) {
	mViewMatrix.setIdentity();

	mFovY = acos(-1.0f) / 3.0;
	mNearDist = 1.;
	mFarDist = 5000.;

	mVpX = 0;
	mVpY = 0;

	setPosition(Vector3f::Constant(100.));
	setTarget(Vector3f::Zero());
}

Camera::Camera(const Camera& other){
	*this = other;
}

Camera::~Camera(){
}

Camera& Camera::operator=(const Camera& other) {
	mViewIsUptodate = false;
	mProjIsUptodate = false;

	mVpX = other.mVpX;
	mVpY = other.mVpY;
	mVpWidth = other.mVpWidth;
	mVpHeight = other.mVpHeight;

	mTarget = other.mTarget;
	mFovY = other.mFovY;
	mNearDist = other.mNearDist;
	mFarDist = other.mFarDist;

	mViewMatrix = other.mViewMatrix;
	mProjectionMatrix = other.mProjectionMatrix;

	return *this;
}

void Camera::setViewport(unsigned int offsetx, unsigned int offsety, unsigned int width, unsigned int height){
	mVpX = offsetx;
	mVpY = offsety;
	mVpWidth = width;
	mVpHeight = height;

	mProjIsUptodate = false;
}

void Camera::setViewport(unsigned int width, unsigned int height){
	mVpWidth = width;
	mVpHeight = height;

	mProjIsUptodate = false;
}

void Camera::setFovY(float value){
	mFovY = value;
	mProjIsUptodate = false;
}

void Camera::setPosition(const Eigen::Vector3f& pos){
	mFrame.position = pos;
	mViewIsUptodate = false;
}

void Camera::setOrientation(const Eigen::Quaternionf& q){
	mFrame.orientation = q;
	mViewIsUptodate = false;
}

void Camera::setFrame(const Frame& f){
	mFrame = f;
	mViewIsUptodate = false;
}

void Camera::setDirection(const Eigen::Vector3f& newDirection){
	Vector3f up = this->up();

	Matrix3f camAxes;
	camAxes.col(2) = (-newDirection).normalized();
	camAxes.col(0) = up.cross(camAxes.col(2)).normalized();
	camAxes.col(1) = camAxes.col(2).cross(camAxes.col(0)).normalized();

	setOrientation(Quaternionf(camAxes));

	mViewIsUptodate = false;

}

Eigen::Vector3f Camera::direction(void) const{
	return -(orientation() * Vector3f::UnitZ());
}

void Camera::setUp(const Eigen::Vector3f& vectorUp){
	
}

Eigen::Vector3f Camera::up(void) const{
	return orientation() * Vector3f::UnitY();
}

Vector3f Camera::right(void) const{
	return orientation() * Vector3f::UnitX();
}

void Camera::setTarget(const Eigen::Vector3f& target){
	mTarget = target;
	if (!mTarget.isApprox(position())) {
		Vector3f newDirection = mTarget - position();
		setDirection(newDirection.normalized());
	}
}

const Eigen::Affine3f& Camera::viewMatrix(void) const{
	updateViewMatrix();
	return mViewMatrix;
}

const Eigen::Matrix4f& Camera::projectionMatrix(void) const{
	updateProjectionMatrix();
	return mProjectionMatrix;
}


// TO UNDERSTAND
void Camera::rotateAroundTarget(const Eigen::Quaternionf& q){
	Matrix4f mrot, mt, mtm;

	updateViewMatrix();
	Vector3f t = mViewMatrix * mTarget;

	mViewMatrix = Translation3f(t) * q * Translation3f(-t) * mViewMatrix;

	Quaternionf qa(mViewMatrix.linear());
	qa = qa.conjugate();
	setOrientation(qa);
	setPosition(-(qa * mViewMatrix.translation()));
	
	mViewIsUptodate = true;


}


// TO UNDERSTAND
void Camera::localRotate(const Eigen::Quaternionf& q){
	float dist = (position() - mTarget).norm();
	setOrientation(orientation() * q);
	mTarget = position() + dist * direction();
	mViewIsUptodate = false;
}


// move d along direction
void Camera::zoom(float d){
	float dist = (position() - mTarget).norm();
	if (dist > d) {
		setPosition(position() + direction() * d);
		mViewIsUptodate = false;
	}
}

// TO BE UNDERSTOOD
void Camera::localTranslate(const Eigen::Vector3f& t){
	Vector3f trans = orientation() * t;
	setPosition(position() + trans);
	setTarget(mTarget + trans);

	mViewIsUptodate = false;
}

void Camera::activateGL(void){
	
}

Eigen::Vector3f Camera::unProject(const Eigen::Vector2f& uv, float depth, const Eigen::Matrix4f& invModelview) const{
	updateViewMatrix();
	updateProjectionMatrix();

	Vector3f a(2. * uv.x() / float(mVpWidth) - 1., 2. * uv.y() / float(mVpHeight) - 1., 1.);
	a.x() *= depth / mProjectionMatrix(0, 0);
	a.y() *= depth / mProjectionMatrix(1, 1);
	a.z() = -depth;
	// FIXME /\/|
	Vector4f b = invModelview * Vector4f(a.x(), a.y(), a.z(), 1.);
	return Vector3f(b.x(), b.y(), b.z());
}

Eigen::Vector3f Camera::unProject(const Eigen::Vector2f& uv, float depth) const{
	
	Matrix4f inv = mViewMatrix.inverse().matrix();
	return unProject(uv, depth, inv);
}

void Camera::updateViewMatrix(void) const{
	if (!mViewIsUptodate) {
		Quaternionf q = orientation().conjugate();
		mViewMatrix.linear() = q.toRotationMatrix();
		mViewMatrix.translation() = -(mViewMatrix.linear() * position());

		mViewIsUptodate = true;
	}
}

void Camera::updateProjectionMatrix(void) const{
	if (!mProjIsUptodate) {
		mProjectionMatrix.setIdentity();
		float aspect = float(mVpWidth) / float(mVpHeight);
		float theta = mFovY * 0.5;
		float range = mFarDist - mNearDist;
		float invtan = 1. / tan(theta);

		mProjectionMatrix(0, 0) = invtan / aspect;
		mProjectionMatrix(1, 1) = invtan;
		mProjectionMatrix(2, 2) = -(mNearDist + mFarDist) / range;
		mProjectionMatrix(3, 2) = -1;
		mProjectionMatrix(2, 3) = -2 * mNearDist * mFarDist / range;
		mProjectionMatrix(3, 3) = 0;

		mProjIsUptodate = true;
	}
}














