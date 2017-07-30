#pragma once


namespace Graphics{
    class Camera;
    class Scene;
}

namespace Graphics{
    class Render
    {
        public:
            virtual ~Render()
            {
            }

            virtual void Draw(const Scene &scene) = 0;

            virtual void SetCamera(const Camera *c)
            {
                m_camera = c;
            }

        protected:
            const Camera *m_camera;
    };

}
