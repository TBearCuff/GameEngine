#ifndef SPRITE_H
#define SPRITE_H

#include "interfaces.h"

class Sprite : public IScreenElement
{
protected:
    // Position and Pixel data
    // -----------------------------
//	ID3DXSprite *m_d3dSprite;			// D3D X Sprite
//	IDirect3DTexture9 *m_pTexture;		// Texture to draw on the Sprite

    Point m_Position, m_Hotspot;		// subtract HS from pos to get origin
    int m_ZOrder;						// the sort order of the sprite
    int m_Width, m_Height;				// dimensions of one frame
    int m_TextureWidth, m_TextureHeight;// dimensions of the entire texture
    int m_CurrentFrame, m_NumFrames;	// current frame and total frames
    bool m_HasColorKey;				// set to true if the sprite has a color key
    bool m_IsVisible;				// set to true if you want the sprite to draw

    // Members that control animation
    // ------------------------------
    bool m_IsPaused;			// set to true if the animation has been paused
    bool m_LoopingAnim;			// set to true if the animation loops
    int m_ElapsedTime;			// the measure of total elapsed time in ms
    int m_MSPerFrame;			// ms per frame (1000 / desired frames per second)

public:
    Sprite();
    virtual ~Sprite() { /*SAFE_RELEASE(m_d3dSprite); SAFE_RELEASE(m_pTexture); */ }

    virtual Point const VGetPos() const { return m_Position; }
    virtual void VSetPos(const Point &loc) { m_Position = loc; }
    virtual void VSetHotspot(const Point &loc) { m_Hotspot = loc; }
    virtual void VSetSize(const Point &size) { m_Width=size.x; m_Height=size.y; }
    virtual Rect const VGetRect() const { return Rect(0,0,m_Width-1,m_Height-1); }
    virtual int const VGetWidth() const { return m_Width; }
    virtual int const VGetHeight() const { return m_Height; }

    // Implementation of the IScreenElement interface
    virtual void VOnUpdate(int deltaMS);
    virtual void VOnRender(double fTime, float fElapsedTime);
    virtual int VGetZOrder() const { return m_ZOrder; }
    virtual void VSetZOrder(int const zOrder) { m_ZOrder = zOrder; }
    virtual bool VIsVisible() const { return m_IsVisible; }
    virtual void VSetVisible(bool visible) { m_IsVisible = visible; }
    virtual bool VOnMsgProc( AppMsg msg ) {Q_UNUSED(msg); return false; }
    virtual bool VOnRestore();

//	void *GetSurface() { return m_d3dSprite; }

    // the method that sets the current frame
    void SetFrame(const int desiredFrame) { m_CurrentFrame = desiredFrame % m_NumFrames; }

    // the method that retrieves the current frame
    int GetFrame() const { return m_CurrentFrame; }

    // the method that returns the number of frames in this animation
    int GetFrameCount() const 	{ return m_NumFrames; }

    void SetAnim(bool isPaused=false, bool looping=true, int msPerFrame=67)
        { m_IsPaused=isPaused; m_LoopingAnim=looping; m_MSPerFrame=msPerFrame; }
};

#endif // SPRITE_H
