/************************************************************************/
// project : graph

// author : jiangsan, school of computer science, CUG.

// e-mail: jiangsan@cug.edu.cn

// date : 2022-7-2

/************************************************************************/

#ifndef COLMAP_SRC_SPHERE_CAMERA_H_
#define COLMAP_SRC_SPHERE_CAMERA_H_

#include <Eigen/Core>

#include "base/camera.h"
#include "util/alignment.h"
#include "util/bitmap.h"
#include "util/types.h"

namespace colmap {

// Normalize a given pixel error to the camera plane.
//
// Normalized points are generated by using image2world of the camera.
// Bearing vectors are useful for points on the spherical camera.
//
// @param with				   Camera width.
// @param height		       Camera height.
// @param image_error	       Error in image plane.
// @return					   Error in camera plane.
double ImagePlaneToCameraPlaneError(const size_t width, const size_t height,
                                    double image_error);

// Convert the error in the camera plane to the pixel error.
//
// Normalized points are generated by using image2world of the camera.
// Bearing vectors are useful for points on the spherical camera.
//
// @param with				   Camera width.
// @param height		       Camera height.
// @param camera_error	       Error in camera plane.
// @return					   Error in image plane.
double CameraPlaneToImagePlaneError(const size_t width, const size_t height,
                                    double camera_error);

// Normalize a given pixel error to the sphere plane.
//
// Normalized points are generated by using image2world of the camera.
// Bearing vectors are useful for points on the spherical camera.
//
// @param with				   Camera width.
// @param height		       Camera height.
// @param image_error	       Error in image plane.
// @return					   Error in sphere plane.
double ImagePlaneToSpherePlaneError(const size_t width, const size_t height,
                                    double image_error);

// Convert the error in the sphere plane to the pixel error.
//
// Normalized points are generated by using image2world of the camera.
// Bearing vectors are useful for points on the spherical camera.
//
// @param with				   Camera width.
// @param height		       Camera height.
// @param sphere_error	       Error in sphere plane.
// @return					   Error in image plane.
double SpherePlaneToImagePlaneError(const size_t width, const size_t height,
                                    double sphere_error);

// Convert normalized points to bearing vectors on sphere.
//
// Normalized points are generated by using image2world of the camera.
// Bearing vectors are useful for points on the spherical camera.
//
// @param normalized_points     Normalized image coordinates.
// @param return		        Unit bearing vectors.
Eigen::Vector3d NormalizedPointToBearingVector(
    const Eigen::Vector2d& normalized_point);

std::vector<Eigen::Vector3d> NormalizedPointsToBearingVectors(
    const std::vector<Eigen::Vector2d>& normalized_points);

// Convert bearing vectors on sphere to normalized points.
//
// Normalized points are generated by using image2world of the camera.
// Bearing vectors are useful for points on the spherical camera.
//
// @param normalized_points     Normalized image coordinates.
// @param return		        Unit bearing vectors.
Eigen::Vector2d BearingVectorToNormalizedPoint(
    const Eigen::Vector3d& bearing_vector);

std::vector<Eigen::Vector2d> BearingVectorsToNormalizedPoints(
    const std::vector<Eigen::Vector3d>& bearing_vectors);

// Convert normalized points to [lon, lat] on sphere.
//
// Normalized points are generated by using image2world of the camera.
//
// @param normalized_points     Normalized image coordinates.
// @param return		        Lon and lat in degree.
Eigen::Vector2d NormalizedPointToLonLat(
    const Eigen::Vector2d& normalized_point);

std::vector<Eigen::Vector2d> NormalizedPointsToLonLats(
    const std::vector<Eigen::Vector2d>& normalized_points);

// Compute focal length from height and fov of pinhole camera.
//
// @param height				Camera height.
// @param field_of_view		    Camera field of view.
double PinholeFocalLength(const int height, const double field_of_view = 45.0);

// Configure pinhole camera from image height and fov.
//
// @param width					Camera width.
// @param height				Camera height.
// @param field_of_view		    Camera field of view.
Camera PinholeCamera(const int width, const int height,
                     const double field_of_view = 45.0);

// Configure pinhole camera from image height and fov.
Camera SphereCamera(const int width, const int height);

// Compute rotation matrix that points to different cubic side.
//
// The coordinate system of sphere camera is defined according to the
// following reference:
//
// Torii A, Havlena M, Pajdla T. From google street view to 3d city
// models[C]//2009 IEEE 12th international conference on computer vision
// workshops, ICCV Workshops. IEEE, 2009: 2188-2195.
const std::unordered_map<int, Eigen::Matrix3d> GetCubicRotations();

// Compute the rotation matrix from the nominal XYZ to the tangent plane
// coordinate system defined by [lon, lat].
const Eigen::Matrix3d GetTangentPlaneRotation(const double lon,
                                              const double lat,
                                              const double rot = 0.0);

// Generate local patch image from spherical image.
//
// @param sphere_camera			Sphere camera.
// @param sphere_bitmap			Sphere bitmap.
// @param rotation				Rotation from pinhole to sphere.
// @param pinhole_camera	    Pinhole camera.
// @param pinhole_bitmap	    Pinhole bitmap.
void SphericalToPatch(const Camera& sphere_camera, const Bitmap& sphere_bitmap,
                      const Eigen::Matrix3d& rotation,
                      const Camera& pinhole_camera, Bitmap& pinhole_bitmap);

// Generate tangent projected image from spherical image.
//
// @param sphere_camera			Sphere camera.
// @param sphere_bitmap			Sphere bitmap.
// @param rotation				Rotation from pinhole to sphere.
// @param pinhole_camera	    Pinhole camera.
// @param pinhole_bitmap	    Pinhole bitmap.
void SphericalToTangent(const Camera& sphere_camera,
                        const Bitmap& sphere_bitmap,
                        const Eigen::Matrix3d& rotation,
                        const Camera& pinhole_camera, Bitmap& pinhole_bitmap);

// Generate pinhole image from spherical image.
//
// @param sphere_camera			Sphere camera.
// @param sphere_bitmap			Sphere bitmap.
// @param sphere_path			Sphere image path.
// @param pinhole_camera	    Pinhole camera.
// @param output_path		    Pinhole output path.
// @param image_ids	            Image ids.
// @param rotations				Rotation matrix.
// @param tangent_proj			Use tangent projection.
// @param return		        Pinhole image paths.
std::vector<std::string> SphericalToPinhole(
    const Camera& sphere_camera, const Bitmap& sphere_bitmap,
    const std::string& sphere_path, const Camera& pinhole_camera,
    const std::string& output_path, const std::vector<int>& image_ids,
    const std::unordered_map<int, Eigen::Matrix3d>& rotations,
    const bool tangent_proj = true);

}  // namespace colmap

#endif  // COLMAP_SRC_SPHERE_CAMERA_H_