//Copyright (C) 2011 by Jason L. McKesson
//This file is licensed by the MIT License.



#include "MatrixStack.h"
#include "include\glm\gtc\matrix_transform.hpp"

namespace glutil
{
  void MatrixStack::rotate( const glm::vec3 axis, float angDegCCW )
  {
    m_currMatrix = glm::rotate(m_currMatrix, angDegCCW, axis);
  }

  void MatrixStack::rotateRadians( const glm::vec3 axisOfRotation, float angRadCCW )
  {
    float fCos = cosf(angRadCCW);
    float fInvCos = 1.0f - fCos;
    float fSin = sinf(angRadCCW);
    float fInvSin = 1.0f - fSin;

    glm::vec3 axis = glm::normalize(axisOfRotation);

    glm::mat4 theMat(1.0f);
    theMat[0].x = (axis.x * axis.x) + ((1 - axis.x * axis.x) * fCos);
    theMat[1].x = axis.x * axis.y * (fInvCos) - (axis.z * fSin);
    theMat[2].x = axis.x * axis.z * (fInvCos) + (axis.y * fSin);

    theMat[0].y = axis.x * axis.y * (fInvCos) + (axis.z * fSin);
    theMat[1].y = (axis.y * axis.y) + ((1 - axis.y * axis.y) * fCos);
    theMat[2].y = axis.y * axis.z * (fInvCos) - (axis.x * fSin);

    theMat[0].z = axis.x * axis.z * (fInvCos) - (axis.y * fSin);
    theMat[1].z = axis.y * axis.z * (fInvCos) + (axis.x * fSin);
    theMat[2].z = (axis.z * axis.z) + ((1 - axis.z * axis.z) * fCos);
    m_currMatrix *= theMat;
  }

  void MatrixStack::rotateX( float angDegCCW )
  {
    rotate(glm::vec3(1.0f, 0.0f, 0.0f), angDegCCW);
  }

  void MatrixStack::rotateY( float angDegCCW )
  {
    rotate(glm::vec3(0.0f, 1.0f, 0.0f), angDegCCW);
  }

  void MatrixStack::rotateZ( float angDegCCW )
  {
    rotate(glm::vec3(0.0f, 0.0f, 1.0f), angDegCCW);
  }

  void MatrixStack::scale( const glm::vec3 &scaleVec )
  {
    m_currMatrix = glm::scale(m_currMatrix, scaleVec);
  }

  void MatrixStack::translate( const glm::vec3 &offsetVec )
  {
    m_currMatrix = glm::translate(m_currMatrix, offsetVec);
  }

  void MatrixStack::perspective( float degFOV, float aspectRatio, float zNear, float zFar )
  {
    m_currMatrix *= glm::perspective(degFOV, aspectRatio, zNear, zFar);
  }

  void MatrixStack::orthographic( float left, float right, float bottom, float top,
    float zNear, float zFar )
  {
    m_currMatrix *= glm::ortho(left, right, bottom, top, zNear, zFar);
  }

  void MatrixStack::pixelPerfectOrtho( glm::ivec2 size, glm::vec2 depthRange, bool isTopLeft /*= true*/ )
  {
    if(isTopLeft)
    {
      translate(-1.0f, 1.0f, (depthRange.x + depthRange.y) / 2.0f);
      scale(2.0f / size.x, -2.0f / size.y, 1.0f);
    }
    else
    {
      translate(-1.0f, -1.0f, (depthRange.x + depthRange.y) / 2.0f);
      scale(2.0f / size.x, 2.0f / size.y, 2.0f / (depthRange.y - depthRange.x));
    }
  }

  void MatrixStack::lookAt( const glm::vec3 &cameraPos, const glm::vec3 &lookatPos, const glm::vec3 &upDir )
  {
    m_currMatrix *= glm::lookAt(cameraPos, lookatPos, upDir);
  }

  void MatrixStack::applyMatrix( const glm::mat4 &theMatrix )
  {
    m_currMatrix *= theMatrix;
  }

  void MatrixStack::setMatrix( const glm::mat4 &theMatrix )
  {
    m_currMatrix = theMatrix;
  }

  void MatrixStack::setIdentity()
  {
    m_currMatrix = glm::mat4(1.0f);
  }
}


