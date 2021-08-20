#pragma once

#include "../core/init.h"
#include "../math/quat.h"
#include "../math/vec3.h"
#include "../shapes/edge.h"
#include "./primitive.h"

typedef union Quad3 {
    struct {
        vec3 top_left, top_right, bottom_right, bottom_left;
    };
    vec3 corners[4];
} Quad3;

void setQuad3FromBox(Quad3 *quad, Box *box) {
    quad->top_left     = box->vertices.corners.back_bottom_left;
    quad->top_right    = box->vertices.corners.back_bottom_right;
    quad->bottom_left  = box->vertices.corners.front_bottom_left;
    quad->bottom_right = box->vertices.corners.front_bottom_right;
    for (u8 i = 0; i < 4; i++) quad->corners[i].y = 0;
}

typedef union Quad4 {
    struct {
        vec4 top_left, top_right, bottom_right, bottom_left;
    };
    vec4 corners[4];
} Quad4;

void setQuad4FromBox(Quad4 *quad, Box *box) {
    quad->top_left     = Vec4fromVec3(box->vertices.corners.back_bottom_left, 1.0f);
    quad->top_right    = Vec4fromVec3(box->vertices.corners.back_bottom_right, 1.0f);
    quad->bottom_left  = Vec4fromVec3(box->vertices.corners.front_bottom_left, 1.0f);
    quad->bottom_right = Vec4fromVec3(box->vertices.corners.front_bottom_right, 1.0f);
    for (u8 i = 0; i < 4; i++) quad->corners[i].y = 0;
}

void transformBoxVerticesFromObjectToViewSpace(Viewport *viewport, Primitive *primitive, BoxVertices *vertices, BoxVertices *transformed_vertices) {
    vec3 position;
    for (u8 i = 0; i < BOX__VERTEX_COUNT; i++) {
        position = vertices->buffer[i];
        position = convertPositionToWorldSpace(position, primitive);
        position = subVec3(    position, viewport->camera->transform.position);
        position = mulVec3Quat(position, viewport->camera->transform.rotation_inverted);
        transformed_vertices->buffer[i] = position;
    }
}

void drawBox(Viewport *viewport, RGBA color, Box *box, Primitive *primitive, u8 sides, u8 line_width) {
    // Transform vertices positions from local-space to world-space and then to view-space:
    static BoxVertices vertices;
    transformBoxVerticesFromObjectToViewSpace(viewport, primitive, &box->vertices, &vertices);

    // Distribute transformed vertices positions to edges:
    static BoxEdges edges;
    setBoxEdgesFromVertices(&edges, &vertices);

    if (sides == BOX__ALL_SIDES) {
        for (u8 i = 0; i < BOX__EDGE_COUNT; i++)
            drawEdge(viewport, color, edges.buffer + i, line_width);
    } else {
        if (sides & Front | sides & Top   ) drawEdge(viewport, color, &edges.sides.front_top, line_width);
        if (sides & Front | sides & Bottom) drawEdge(viewport, color, &edges.sides.front_bottom, line_width);
        if (sides & Front | sides & Left  ) drawEdge(viewport, color, &edges.sides.front_left, line_width);
        if (sides & Front | sides & Right ) drawEdge(viewport, color, &edges.sides.front_right, line_width);
        if (sides & Back  | sides & Top   ) drawEdge(viewport, color, &edges.sides.back_top, line_width);
        if (sides & Back  | sides & Bottom) drawEdge(viewport, color, &edges.sides.back_bottom, line_width);
        if (sides & Back  | sides & Left  ) drawEdge(viewport, color, &edges.sides.back_left, line_width);
        if (sides & Back  | sides & Right ) drawEdge(viewport, color, &edges.sides.back_right, line_width);
        if (sides & Left  | sides & Top   ) drawEdge(viewport, color, &edges.sides.left_top, line_width);
        if (sides & Left  | sides & Bottom) drawEdge(viewport, color, &edges.sides.left_bottom, line_width);
        if (sides & Right | sides & Top   ) drawEdge(viewport, color, &edges.sides.right_top, line_width);
        if (sides & Right | sides & Bottom) drawEdge(viewport, color, &edges.sides.right_bottom, line_width);
    }
}

void drawCamera(Viewport *viewport, RGBA color, Camera *camera, u8 line_width) {
    static Box box;
    static Primitive primitive;
    initBox(&box);
    primitive.flags = ALL_FLAGS;
    primitive.rotation = camera->transform.rotation;
    primitive.position = camera->transform.position;
    primitive.scale.x  = primitive.scale.y = primitive.scale.z = 1;
    drawBox(viewport, color, &box, &primitive, BOX__ALL_SIDES, line_width);
    box.vertices.corners.back_bottom_left   = scaleVec3(box.vertices.corners.back_bottom_left,  0.5f);
    box.vertices.corners.back_bottom_right  = scaleVec3(box.vertices.corners.back_bottom_right, 0.5f);
    box.vertices.corners.back_top_left      = scaleVec3(box.vertices.corners.back_top_left,     0.5f);
    box.vertices.corners.back_top_right     = scaleVec3(box.vertices.corners.back_top_right,    0.5f);
    box.vertices.corners.front_bottom_left  = scaleVec3(box.vertices.corners.front_bottom_left,  2);
    box.vertices.corners.front_bottom_right = scaleVec3(box.vertices.corners.front_bottom_right, 2);
    box.vertices.corners.front_top_left     = scaleVec3(box.vertices.corners.front_top_left,     2);
    box.vertices.corners.front_top_right    = scaleVec3(box.vertices.corners.front_top_right,    2);
    for (u8 i = 0; i < BOX__VERTEX_COUNT; i++)
        box.vertices.buffer[i].z += 1.5f;
    drawBox(viewport, color, &box, &primitive, BOX__ALL_SIDES, line_width);
}