#pragma once

#include "light.h"
#include<vector>
#include <memory>


namespace Graphics{
    class Surface;

    class Scene{
        public:
            Scene() = default;
            ~Scene() = default;

            inline void SetDiffuseLight(const DiffuseLight &d){
                m_diffuseLight = d;
            }

            inline const DiffuseLight &GetDiffuseLight() const{
                return m_diffuseLight;
            }

            inline void AddSpecularLight(const SpecularLight &s){
                m_specularLightList.push_back(s);
            }

            inline const std::vector<SpecularLight> &GetSpecularLightList() const{
                return m_specularLightList;
            }

            inline void SetAmbientLight(const AmbientLight &a){
                m_ambientLight = a;
            }

            inline const AmbientLight &GetAmbientLight() const{
                return m_ambientLight;
            }

            inline void AddSurface(std::shared_ptr<Surface> surface){
                m_surfaces.push_back(surface);
            }

            inline const std::vector<std::shared_ptr<Surface>> &GetSurfaces() const{
                return m_surfaces;
            }

        private:
            DiffuseLight m_diffuseLight;
            std::vector<SpecularLight> m_specularLightList;
            AmbientLight m_ambientLight;
            std::vector<std::shared_ptr<Surface>> m_surfaces;
    };
}
