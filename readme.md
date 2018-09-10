  								BIENVENIDOS AL SHELL
  Manual Proyecto final Operative System

  los comandos que se implementados en este proyecto son:
	*Clr -------> Limpia la pantalla

  *environ ---> Muestra todas las variables de ambiente que se encuentra en nuestro shell

  *Pause -----> Pausa el shell y no continua hasta que escribamos "pause"

  *cd <directory> -------> cambia el directorio donde nos encontremos por el nombre a donde queremos ir "<directory>"

  *dir -------> nos muestra los archivos que se encuentran en la carpeta donde estemos ubicados

  *echo <message> ------>  nos muestra en pantalla el mensaje que le pasemos por parametro

  *help -------> muestra el manual de informacion del shell

  *quit ------> cierra el shell*

	Esos son algunos de los comandos implementados en el shell, sin embargo,
  podemos invocar otros comandos ya que la implementacion de como corren los comandos se hizo
  con fork y exe. Esto nos permite que otros comandos externos puedan
  ejecutarse en el shell sin necesidad de que esten programados en nuestro shell.

  Para correr el shell es necesario estar en la carpeta donde se encuentra nuestro "Codigo.c"
  y que junto a este se encuentre el archivo "makefile". abrimos la terminal y escribimos el
  comando "make", a continuacion nuestro codigo se compilara y estaremos listo para ejecutar
  nuestro shell. en la consola escribimos "./NombreDelEjecutable", esto nos abrira nuestro shell
  y podremos ejecutar los comandos que al principio se presentaron.

	ejemplo (ejecutor los comandos abajos en el promnt):

      *make

      *./nombreDeNuestroEjecutable

      * "==> " (este es nuestro shell)

      *En nuestro shell escribimos cualquier de los comandos ya descritos

      *quit para cerrar nuestro shell*

  Otra de las ventajas que tenemos en el shell es que podemos realizar input/output redirection
  (Redireccionar entradas y salidas) de nuestro shell, es decir, podemos correr programas sin nececidad
  de escribirlos, solo basta con que los comando que queramos a escribir esten en un archivo ".txt"
  escritos con unas cualidades especificas, es decir, con sus respectivos espacios y tomar en
  cuenta el case sensitive. a continuacion mostraremos como hacer I/O redirection.



	Output redirection:
		En nuestro shell, escribiremos:

            *parametro1 > nombreDelArchivo

            *parametro1 parametro2 > nombreDelArchivo*

    donde nuestros parametros son los comandos que queramos correr. y el nombre del
    archivo es el nombre del archivo que queremos crear si no esta creado o sobreescribir
    si existe. Tener en cuenta que el output redirection solo funciona cuando 2 parametros
    se envian al archivo que creamos

	input redirection:
		antes de correr nuestro shell, debemos saber de que archivo de texto vamos a correr los
    comando	en nuestro shell. Cuando sepamos esto, nos bastara con, antes de correr nuestro
    shell, poner el ejecutable de nuestro shell y hacer el redirection, ejemplo:

    				* ./NombreDeNuestroEjecutableShell < ArchivoConComandos*

		debemos tomar en cuenta que el archivo con los comandos debe estar escrito correctamente y
		de que si hay un comando que no exista probocara un error interno.
