#ifndef EIGEN_CAMERA_H
#define EIGEN_CAMERA_H

#include <Eigen/Geometry>


class Frame {
public:
	inline Frame(const Eigen::Vector3f& pos = Eigen::Vector3f::Zero(), const Eigen::Quaternionf& o = Eigen::Quaternionf()):orientation(o), position(pos) {}

	Frame lerp(float alpha, const Frame& other) const {
		return Frame((1.f - alpha) * position + alpha * other.position, orientation.slerp(alpha, other.orientation));
	}


	Eigen::Quaternionf orientation;
	Eigen::Vector3f position;

};

class Camera {
public:
	Camera();
	Camera(const Camera& other);

	virtual ~Camera();
	Camera& operator=(const Camera& other);

    void setViewport(unsigned int offsetx, unsigned int offsety, unsigned int width, unsigned int height);
    void setViewport(unsigned int width, unsigned int height);

    inline unsigned int vpX(void) const { return mVpX; }
    inline unsigned int vpY(void) const { return mVpY; }
    inline unsigned int vpWidth(void) const { return mVpWidth; }
    inline unsigned int vpHeight(void) const { return mVpHeight; }

    inline float fovY(void) const { return mFovY; }
    void setFovY(float value);

    void setPosition(const Eigen::Vector3f& pos);
    inline const Eigen::Vector3f& position(void) const { return mFrame.position; }

    void setOrientation(const Eigen::Quaternionf& q);
    inline const Eigen::Quaternionf& orientation(void) const { return mFrame.orientation; }

    void setFrame(const Frame& f);
    const Frame& frame(void) const { return mFrame; }

    void setDirection(const Eigen::Vector3f& newDirection);
    Eigen::Vector3f direction(void) const;
    void setUp(const Eigen::Vector3f& vectorUp);
    Eigen::Vector3f up(void) const;
    Eigen::Vector3f right(void) const;

    void setTarget(const Eigen::Vector3f& target);
    inline const Eigen::Vector3f& target(void) { return mTarget; }

    const Eigen::Affine3f& viewMatrix(void) const;
    const Eigen::Matrix4f& projectionMatrix(void) const;

    void rotateAroundTarget(const Eigen::Quaternionf& q);
    void localRotate(const Eigen::Quaternionf& q);
    void zoom(float d);

    void localTranslate(const Eigen::Vector3f& t);

    /** Setup OpenGL matrices and viewport */
    void activateGL(void);

    Eigen::Vector3f unProject(const Eigen::Vector2f& uv, float depth, const Eigen::Matrix4f& invModelview) const;
    Eigen::Vector3f unProject(const Eigen::Vector2f& uv, float depth) const;

protected:
    void updateViewMatrix(void) const;
    void updateProjectionMatrix(void) const;

protected:
	unsigned int mVpX, mVpY;
	unsigned int mVpWidth, mVpHeight;

	Frame mFrame;
	mutable Eigen::Affine3f mViewMatrix;
	mutable Eigen::Matrix4f mProjectionMatrix;

	mutable bool mViewIsUptodate;
	mutable bool mProjIsUptodate;

	Eigen::Vector3f mTarget;	// used by rotateAroundTarget

	float mFovY;
	float mNearDist;
	float mFarDist;
};





#endif // !EIGEN_CAMERA_H
