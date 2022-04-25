
# Cómo ejecutar el ejemplo Ping Pong

Suponemos que ya has descargado el proyecto (Visual Studio) del juego, y lo has descomprimido en  en la carpeta C:\hlocal\TPV2.  Necesitamos ejecutar el programa 2 veces (una para cada cliente). Esto no se puede hacer  desde visual studio, así que vamos a ejecutarlo de otra forma.


**Usando un script BAT:**

Una forma de ejecutar el programa es usar un script ``BAT``. Edita el archivo ``C:\hlocal\TPV2\pingpong.bat`` y cambia ``c:\hlocal\TPV2\TPV2`` a la ruta de tu directorio TPV2 (el que incluye ``src``). Ahora, para cada jugador, puedes simplemente hacer clic sobre ``pingpong.bat`` (desde un ``File Explorer`` como cualquier otro programa) y seguir las instrucciones.


**Usando una consola:**

En este método, simplemente ejecutamos las instruciones que están en ``pingpong.bat`` desde una consola. Para cada jugador, abre una consola (ver abajo como hacerlo) y ejecuta los siguientes comandos y seguir las instrucciones:

```
  c:
  cd c:\hlocal\TPV2\TPV2
  ..\bin\TPV2Debug.exe 
```

Notáse que tenemos que estar en el directorio ``c:\hlocal\TPV2\TPV2`` porque es el directorio que incluye el directorio ``resources``, y el programa asume que ``resources`` está en el directorio actual -- cuando ejecutamos un programa desde Visual Studio, esto se hace automáticamente porque en la configuración del proyecto hay una opción llamada *directorio de trabajo (working directory)* que ya está configurada con el valor ``c:\hlocal\TPV2\TPV2``.


# Cómo abrir una consola

- Pulsa la tecla ``Windows+R`` (o abre el menú ``Start`` y haz click en la opción ``Run``)
- Escribe en el recuadro ``cmd`` y haz clic en ``OK`` (o pulsar ``ENTER``)  


Mas información : [https://www.howtogeek.com/235101/10-ways-to-open-the-command-prompt-in-windows-10/](https://www.howtogeek.com/235101/10-ways-to-open-the-command-prompt-in-windows-10/).

