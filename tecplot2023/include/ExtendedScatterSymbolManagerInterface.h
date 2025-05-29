#pragma once

#include "ThirdPartyHeadersBegin.h"
    #include <string>
    #include <vector>
#include "ThirdPartyHeadersEnd.h"

namespace tecplot
{
class ExtendedScatterSymbolManagerInterface
{
public:
    enum class PolygonBlockType
    {
        Triangles,
        TriangleStrip,
        TriangleFan, /* default */
    };

    /**
     * struct Polyline contains all of the information needed to draw either a line of points
     * or a filled polygon
     */
    struct Polyline
    {
        /**
         * Filled polygons cannot be outlined. To outline, the client should add a separate non-filled line.
         */
        bool                m_isFilled;
        PolygonBlockType    m_polygonBlockType;     // If a block type other than TrangleFan is needed for polygons, it can be set here.
        ColorIndex_t        m_color;         // Fill color if filled, line color if not.
        double              m_lineThickness; // Line thickness if not filled, ignored otherwise.
        double              m_lift;          // Lift allows the client to separate intersecting layers of lines if needed when drawn in 3D.

        /**
         * Vector of XY points in the line in normalized space 0.0 <= X <= 1.0 and 0.0 <= Y <= 1.0.
         * The client may draw outside of the normalized (0.0, 1.0) space, but
         * this will cause the scatter symbol to be larger that the engine scatter symbol width value.
         *
         * Note that if the last point does not equal the first point and isFilled is true, a last point will be added which
         * is equal to the first point.
         */
        std::vector<XY_s>   m_points;

        Polyline() :
            m_isFilled(false)
            , m_polygonBlockType(PolygonBlockType::TriangleFan)
            , m_color(Black_C)
            , m_lineThickness(.001)
            , m_lift(0.0)
        {}
    };

    using PolylineVector_t = std::vector<Polyline>;
    using PolylineVectorProvider_pf = PolylineVector_t(*)(
        EntIndex_t      zone, 
        LgIndex_t       nodeIndex, 
        ColorIndex_t    backgroundColor, // from Frame.GlobalScatter, provided as a convenience for the client.
        ColorIndex_t    lineColor,       // ""
        double          lineThickness);  // ""

    /**
     * Register a line art provider.
     * The name is case-INsensitive, and is specified in $!GlobalScatter LineArtProvider=<string>
     *
     * @param providerName
     *  Name of the line art provider. The name cannot be the same as an existing registered provider,
     *  and must be non-empty.
     *
     * @param polylineProviderCallback
     *  Function which will be called to provide the line art.
     *
     * @return
     *  true if successful, false if the provider name has already been registered.
     */
    virtual bool registerLineArtProviderCallback(
        std::string const& providerName, 
        PolylineVectorProvider_pf polylineProviderCallback) = 0;

    /**
     * The client should call this function to invalidate and redraw all
     * scatter symbols.
     */
    virtual void definitionChanged(std::string const& providerName) = 0;

    /**
     * Set the aspect ratio for all scatter symbols for this provider.
     *
     * @param providerName
     *  Provider name
     *
     * @param aspectRatio
     *  Aspect ratio for all scatter symbols rendered by the provider.
     *  must be > 0.0
     */
    virtual void setAspectRatio(
        std::string const& providerName, 
        double aspectRatio) = 0;

    /**
     * Set the anchor point of the scatter symbol.
     * Default is (0, 0), which is the lower left
     * corner of the bounding rectangle.
     *
     * The anchor point is normalized (0.0, 1.0),
     * so for example setting to (.5, .5) would place the scatter symbol
     * in the center of the bounding rectangle.
     *
     * @param providerName
     *  Provider name
     *
     * @param anchorX
     *  X coordinate of anchor point.
     *
     * @param anchorY
     *  Y coordinate of anchor point.
     */
    virtual void setAnchorPoint(
        std::string const& providerName,
        double anchorX,
        double anchorY) = 0;

    virtual ~ExtendedScatterSymbolManagerInterface() {}
};
}
