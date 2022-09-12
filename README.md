# RayTracer

### How it works
* we see colour because light (originating from a source) is reflected off of various subjects, before hitting our retina
* when rendering 2D images, we can also think of it as rays originating from our eye (i.e. the observer), hitting various objects -- this is the idea of raytracing used here
* images are created in ppm format (used an online converter to .jpg to display on this page)

### Features

##### Shapes
* can generate spheres, rectangular prisms, individual rectangular planes, and triangular planes
<img src=https://user-images.githubusercontent.com/79111376/189561709-5e3f24ba-0bc5-4a49-81e4-f30c3e9adbb8.jpg width="300">

##### Textures
* specify an image file, and the texture on here will be wrapped around each 3d object, as shown in the above image

##### Materials
* can have objects with various shapes and textures with the materials : glass, metal, diffuse
* can specify how diffuse the reflection will be for metal and diffuse objects, and can specify the index of refraction for glass
<img src="https://user-images.githubusercontent.com/79111376/189561951-000827d3-9e7a-4846-991e-05ebc94fdc53.jpg" width="300">
(diffuse on ground, metal on left and center, glass on right)

##### Position the camera
* the camera will always point straight towards the screen, but its position can be adjusted
<img src="https://user-images.githubusercontent.com/79111376/189563092-fb82e59f-ca97-4642-aa80-662c867c58a5.jpg" width="300">
(same object positioning as above, but camera to the right + further back)

##### Lights
* lights with different colours, intensities, and sizes can be added
* each light will act like a point light, with intensity decreasing radially
<img src="https://user-images.githubusercontent.com/79111376/189562197-dbb8fd02-df89-44b3-abfc-93339c5b0869.jpg" width="300">
(metal balls in colourful lighting :tada:)

##### Resolution and Dimensions
* specify width/height ratio, and number of pixels in width -- a greater resolution will take longer
<img src="https://user-images.githubusercontent.com/79111376/189562512-696ce4f4-673f-4e8c-a25a-3e67c406ee48.jpg" height="300">
