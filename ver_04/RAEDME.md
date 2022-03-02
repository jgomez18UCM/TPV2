
This version uses a simple concept of components to change the behavior of the paddles/ball. We define a 3 kinds of component interfaces (Input, Render and Physics) and a corresponding class Container (that's a game object) that includes vectors of such components. When calling hanldeInput/update/render of a container the calls are delegated to the corresponding components. 

