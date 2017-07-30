TODO:
    reorganize the project.
        (relation between raytracer, math, google-test.Now makefile use absolute path of google-test and the git repo of raytracer/include has a symble-link for math)

    code:
        1.performance improving
        2.use visitor pattern to reorganize the RayTracerSurfaceInterface
    
    feature:
        common:
            1.lua for configuration
            2.complex model render 

        raytracer::
            1.sphere intersect test code and add sphere rendering
            2.simple shadow
            3.ideal specular reflection

        rasterize:
            1.clipping
            2.texture-mapping
            3.scane-line - zbuffer
            4.per-fragment-shading
            5.culling
            6.simple anti-alising
