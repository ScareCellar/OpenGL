#pragma once
#include "Vector2.h"
#include "Core/Serializable.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

namespace neu {
    
    struct Transform : public ISerializable {
        
        glm::vec3 position{ 0, 0, 0 };
        glm::vec3 rotation{ 0,0,0 };
        glm::vec3 scale{ 1,1,1 };

        Transform() = default;
        Transform(const glm::vec3& position, const glm::vec3& rotation = { 0,0,0 }, const glm::vec3& scale = { 1,1,1 }) :
            position{ position },
            rotation{ rotation },
            scale{ scale }
        {
        }

        glm::mat4 GetMatrix() const {
            glm::mat4 mx = glm::mat4(1.0f);
            mx = glm::translate(mx, position);
            mx *= glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
            mx = 1;//scale;
        }
        /// <summary>
        /// Rotation angle in radians around the object's center.
        /// 
        /// Defines the angular orientation of the object in 2D space. The rotation
        /// is measured counter-clockwise from the positive X-axis, following standard
        /// mathematical convention.
        /// 
        /// Angular Measurements:
        /// - 0 radians: Pointing right (positive X direction)
        /// - π/2 radians (90°): Pointing up (positive Y direction)
        /// - π radians (180°): Pointing left (negative X direction)  
        /// - 3π/2 radians (270°): Pointing down (negative Y direction)
        /// 
        /// Range: Typically kept in [0, 2π] or [-π, π] range, though any value is valid
        /// Default value: 0 (no rotation, pointing right)
        /// 
        /// Common uses:
        /// - Sprite orientation (character facing direction)
        /// - Projectile trajectory angles
        /// - UI element rotation effects
        /// - Camera rotation for tilted views
        /// - Animation rotation keyframes
        /// 
        /// Note: If your coordinate system uses Y-down (common in computer graphics),
        /// positive rotation may appear clockwise on screen.
        /// </summary>
        //float rotation = 0;

        /// <summary>
        /// Uniform scale factor applied to both X and Y dimensions.
        /// 
        /// Controls the size of the object relative to its original dimensions.
        /// This is a uniform scale, meaning the same factor is applied to both
        /// width and height, preserving the object's aspect ratio.
        /// 
        /// Scale Values:
        /// - 1.0: Original size (no scaling)
        /// - > 1.0: Larger than original (e.g., 2.0 = double size)
        /// - 0.0 < scale < 1.0: Smaller than original (e.g., 0.5 = half size)
        /// - 0.0: Invisible/degenerate (not recommended)
        /// - < 0.0: Flipped and scaled (negative values flip the object)
        /// 
        /// Default value: 1 (original size, no scaling)
        /// 
        /// Common uses:
        /// - Object size variations (power-ups, different enemy sizes)
        /// - Animation effects (growing, shrinking, pulsing)
        /// - UI scaling for different screen resolutions
        /// - Zoom effects and magnification
        /// - Size-based game mechanics (small vs large objects)
        /// 
        /// Performance note: Non-uniform scaling (different X/Y factors) would
        /// require a more complex transformation representation but is not
        /// supported by this simple Transform structure.
        /// </summary>
        //float scale = 1;

        /// <summary>
        /// Default constructor creating an identity transform.
        /// 
        /// Creates a Transform with default values:
        /// - Position: (0, 0) - at world origin
        /// - Rotation: 0 radians - no rotation
        /// - Scale: 1 - original size
        /// 
        /// This represents the identity transformation, where objects appear
        /// at the origin with no modifications to their original state.
        /// </summary>
        //Transform() = default;

        /// <summary>
        /// Parameterized constructor for creating transforms with specified values.
        /// 
        /// Allows creation of Transform objects with custom position, rotation, and scale
        /// in a single constructor call. Rotation and scale parameters are optional,
        /// defaulting to no rotation and original scale if not specified.
        /// 
        /// This constructor is particularly useful for:
        /// - Setting up initial object positions
        /// - Creating transforms from known values
        /// - Factory methods that create positioned objects
        /// - Animation keyframe initialization
        /// 
        /// Example usage:
        /// ```cpp
        /// Transform playerTransform({100.0f, 200.0f}, math::pi / 4, 1.5f);  // Position, 45° rotation, 1.5x scale
        /// Transform simpleTransform({50.0f, 75.0f});  // Position only, default rotation and scale
        /// ```
        /// </summary>
        /// <param name="position">The 2D position in world coordinates</param>
        /// <param name="rotation">The rotation angle in radians (default: 0 = no rotation)</param>
        /// <param name="scale">The uniform scale factor (default: 1 = original size)</param>
        /*Transform(const vec2& position, float rotation = 0, float scale = 1) :
            position{ position },
            rotation{ rotation },
            scale{ scale }
        {
        }*/

        /// <summary>
        /// Deserializes transform data from serialized format.
        /// 
        /// Implements the ISerializable interface to load Transform data from
        /// configuration files, save games, or other serialized sources.
        /// This enables data-driven object placement and transform configuration.
        /// 
        /// Expected serialized format:
        /// ```json
        /// {
        ///     "position": {x: 100.0, y: 200.0},
        ///     "rotation": 1.5708,  // π/2 radians (90 degrees)
        ///     "scale": 1.5
        /// }
        /// ```
        /// 
        /// The method uses SERIAL_READ macros to safely extract data with
        /// default values for missing properties:
        /// - Missing position defaults to (0, 0)
        /// - Missing rotation defaults to 0 radians
        /// - Missing scale defaults to 1.0
        /// 
        /// This robust loading ensures transforms work even with incomplete
        /// or legacy data files, providing graceful degradation.
        /// 
        /// Common uses:
        /// - Loading object positions from level definition files
        /// - Restoring transform state from save games
        /// - Reading transform data from configuration files
        /// - Deserializing network transform updates
        /// </summary>
        /// <param name="value">Serialized data containing transform information</param>
        void Read(const serial_data_t& value) override;
    };
}