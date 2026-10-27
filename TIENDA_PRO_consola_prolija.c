#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

/* ============================================================================
   ESTRUCTURAS DE DATOS
   ============================================================================ */

typedef struct {
    int id;
    char dni[15];
    char nombre[50];
    char apellido[50];
    char telefono[20];
    char direccion[100];
    int rol; /* 1: Administrador, 2: Repositor, 3: Cliente */
    char usuario[30];
    char contrasena[30];
} Usuario;

typedef struct {
    int id;
    char nombre[30];
    float porcentajeRecargo;
} Categoria;

typedef struct {
    int id;
    char razonSocial[50];
    int origen; /* 1: Nacional, 2: Importado */
    int categorias[10];
    int cantidadCategorias;
    char usuario[30];
    char contrasena[30];
} Proveedor;

typedef struct {
    char codigo[10];
    char nombre[50];
    int idCategoria;
    float costoCompra;
    float precioVenta;
    int stockS;
    int stockM;
    int stockL;
} Prenda;

typedef struct {
    float cajaTotal;
    float cajaEfectivo;
    float cajaTarjeta;
    int totalFacturas;
} Caja;




/* ============================================================================
   PERSISTENCIA EN ARCHIVOS DE TEXTO
   Los archivos se guardan en la carpeta desde la que se ejecuta el programa.
   ============================================================================ */

int separarCampos(char *linea, char *campos[], int maxCampos) {
    int cantidad = 0;
    char *token = strtok(linea, "|\r\n");
    while (token != NULL && cantidad < maxCampos) {
        campos[cantidad++] = token;
        token = strtok(NULL, "|\r\n");
    }
    return cantidad;
}

void guardarUsuarios(Usuario usuarios[], int cantidad) {
    FILE *archivo = fopen("usuarios.txt", "w");
    int i;
    if (archivo == NULL) { printf("[!] No se pudo guardar usuarios.txt\n"); return; }
    for (i = 0; i < cantidad; i++)
        fprintf(archivo, "%d|%s|%s|%s|%s|%s|%d|%s|%s\n", usuarios[i].id,
                usuarios[i].dni, usuarios[i].nombre, usuarios[i].apellido,
                usuarios[i].telefono, usuarios[i].direccion, usuarios[i].rol,
                usuarios[i].usuario, usuarios[i].contrasena);
    fclose(archivo);
}

int cargarUsuarios(Usuario usuarios[], int maximo) {
    FILE *archivo = fopen("usuarios.txt", "r");
    char linea[512], *c[10]; int n = 0, campos;
    if (archivo == NULL) return 0;
    while (n < maximo && fgets(linea, sizeof(linea), archivo)) {
        campos = separarCampos(linea, c, 10);
        if (campos >= 9) {
            usuarios[n].id = atoi(c[0]);
            snprintf(usuarios[n].dni, sizeof(usuarios[n].dni), "%s", c[1]);
            snprintf(usuarios[n].nombre, sizeof(usuarios[n].nombre), "%s", c[2]);
            snprintf(usuarios[n].apellido, sizeof(usuarios[n].apellido), "%s", c[3]);
            snprintf(usuarios[n].telefono, sizeof(usuarios[n].telefono), "%s", c[4]);
            snprintf(usuarios[n].direccion, sizeof(usuarios[n].direccion), "%s", c[5]);
            usuarios[n].rol = atoi(c[6]);
            snprintf(usuarios[n].usuario, sizeof(usuarios[n].usuario), "%s", c[7]);
            snprintf(usuarios[n].contrasena, sizeof(usuarios[n].contrasena), "%s", c[8]);
            n++;
        }
    }
    fclose(archivo); return n;
}

void guardarCategorias(Categoria datos[], int cantidad) {
    FILE *archivo = fopen("categorias.txt", "w"); int i;
    if (!archivo) { printf("[!] No se pudo guardar categorias.txt\n"); return; }
    for (i=0;i<cantidad;i++) fprintf(archivo,"%d|%s|%.2f\n",datos[i].id,datos[i].nombre,datos[i].porcentajeRecargo);
    fclose(archivo);
}
int cargarCategorias(Categoria datos[], int maximo) {
    FILE *archivo=fopen("categorias.txt","r"); char linea[256],*c[4]; int n=0;
    if(!archivo) return 0;
    while(n<maximo && fgets(linea,sizeof(linea),archivo)) if(separarCampos(linea,c,4)>=3) {
        datos[n].id=atoi(c[0]); snprintf(datos[n].nombre,sizeof(datos[n].nombre),"%s",c[1]);
        datos[n].porcentajeRecargo=(float)atof(c[2]); n++;
    }
    fclose(archivo); return n;
}

void guardarProveedores(Proveedor datos[], int cantidad) {
    FILE *archivo=fopen("proveedores.txt","w"); int i,j;
    if(!archivo) { printf("[!] No se pudo guardar proveedores.txt\n"); return; }
    for(i=0;i<cantidad;i++) {
        fprintf(archivo,"%d|%s|%d|%d",datos[i].id,datos[i].razonSocial,datos[i].origen,datos[i].cantidadCategorias);
        for(j=0;j<10;j++) fprintf(archivo,"|%d", j<datos[i].cantidadCategorias ? datos[i].categorias[j] : 0);
        fprintf(archivo,"|%s|%s\n",datos[i].usuario,datos[i].contrasena);
    }
    fclose(archivo);
}
int cargarProveedores(Proveedor datos[], int maximo) {
    FILE *archivo=fopen("proveedores.txt","r"); char linea[512],*c[20]; int n=0,k,campos;
    if(!archivo) return 0;
    while(n<maximo && fgets(linea,sizeof(linea),archivo)) {
        campos=separarCampos(linea,c,20);
        if(campos>=16) {
            datos[n].id=atoi(c[0]); snprintf(datos[n].razonSocial,sizeof(datos[n].razonSocial),"%s",c[1]);
            datos[n].origen=atoi(c[2]); datos[n].cantidadCategorias=atoi(c[3]);
            if(datos[n].cantidadCategorias<0) datos[n].cantidadCategorias=0;
            if(datos[n].cantidadCategorias>10) datos[n].cantidadCategorias=10;
            for(k=0;k<10;k++) datos[n].categorias[k]=atoi(c[4+k]);
            snprintf(datos[n].usuario,sizeof(datos[n].usuario),"%s",c[14]);
            snprintf(datos[n].contrasena,sizeof(datos[n].contrasena),"%s",c[15]);
            n++;
        }
    }
    fclose(archivo); return n;
}

void guardarPrendas(Prenda datos[], int cantidad) {
    FILE *archivo=fopen("prendas.txt","w"); int i;
    if(!archivo) { printf("[!] No se pudo guardar prendas.txt\n"); return; }
    for(i=0;i<cantidad;i++) fprintf(archivo,"%s|%s|%d|%.2f|%.2f|%d|%d|%d\n",datos[i].codigo,datos[i].nombre,datos[i].idCategoria,datos[i].costoCompra,datos[i].precioVenta,datos[i].stockS,datos[i].stockM,datos[i].stockL);
    fclose(archivo);
}
int cargarPrendas(Prenda datos[], int maximo) {
    FILE *archivo=fopen("prendas.txt","r"); char linea[512],*c[10]; int n=0;
    if(!archivo) return 0;
    while(n<maximo && fgets(linea,sizeof(linea),archivo)) if(separarCampos(linea,c,10)>=8) {
        snprintf(datos[n].codigo,sizeof(datos[n].codigo),"%s",c[0]); snprintf(datos[n].nombre,sizeof(datos[n].nombre),"%s",c[1]);
        datos[n].idCategoria=atoi(c[2]); datos[n].costoCompra=(float)atof(c[3]); datos[n].precioVenta=(float)atof(c[4]);
        datos[n].stockS=atoi(c[5]); datos[n].stockM=atoi(c[6]); datos[n].stockL=atoi(c[7]); n++;
    }
    fclose(archivo); return n;
}

void guardarCaja(Caja caja[]) {
    FILE *archivo=fopen("caja.txt","w");
    if(!archivo) { printf("[!] No se pudo guardar caja.txt\n"); return; }
    fprintf(archivo,"%.2f|%.2f|%.2f|%d\n",caja[0].cajaTotal,caja[0].cajaEfectivo,caja[0].cajaTarjeta,caja[0].totalFacturas);
    fclose(archivo);
}
int cargarCaja(Caja caja[]) {
    FILE *archivo=fopen("caja.txt","r"); int ok;
    if(!archivo) return 0;
    ok=fscanf(archivo,"%f|%f|%f|%d",&caja[0].cajaTotal,&caja[0].cajaEfectivo,&caja[0].cajaTarjeta,&caja[0].totalFacturas);
    fclose(archivo); return ok==4;
}

void guardarTodosLosDatos(Usuario usuarios[], int cantUsuarios, Categoria categorias[], int cantCategorias,
                          Proveedor proveedores[], int cantProv, Prenda prendas[], int cantPrendas, Caja caja[]) {
    guardarUsuarios(usuarios,cantUsuarios); guardarCategorias(categorias,cantCategorias);
    guardarProveedores(proveedores,cantProv); guardarPrendas(prendas,cantPrendas); guardarCaja(caja);
}

void registrarVentaArchivo(char tipoFac, const char cliente[], const char codigo[], const char producto[],
                           char talle, int cantidad, float total, const char medioPago[]) {
    FILE *archivo=fopen("ventas.txt","a");
    if(!archivo) { printf("[!] No se pudo registrar la venta en ventas.txt\n"); return; }
    fprintf(archivo,"%ld|%c|%s|%s|%s|%c|%d|%.2f|%s\n",(long)time(NULL),tipoFac,cliente,codigo,producto,talle,cantidad,total,medioPago);
    fclose(archivo);
}


/* ============================================================================
   FUNCIONES GENERALES
   ============================================================================ */

void pausarLimpiar() {
    printf("\n  >> Presione ENTER para continuar...");
    getchar();
    getchar();
}

/* Presentacion visual: ancho adaptable y cuadros centrados. */
#define ANCHO_PANEL 68

int anchoConsola(void) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
        return info.srWindow.Right - info.srWindow.Left + 1;
#endif
    return 100;
}

void margenPanel(void) {
    int espacios = (anchoConsola() - ANCHO_PANEL) / 2;
    int i;
    if (espacios < 0) espacios = 0;
    for (i = 0; i < espacios; i++) putchar(' ');
}

void lineaPanel(void) {
    int i;
    margenPanel(); putchar('+');
    for (i = 0; i < ANCHO_PANEL - 2; i++) putchar('-');
    puts("+");
}

void textoPanel(const char *texto) {
    int n = (int)strlen(texto);
    int i, espacio;
    if (n > ANCHO_PANEL - 4) n = ANCHO_PANEL - 4;
    espacio = ANCHO_PANEL - 2 - n;
    margenPanel(); printf("| %.*s", n, texto);
    for (i = 0; i < espacio - 1; i++) putchar(' ');
    puts("|");
}

void tituloPanel(const char *titulo) {
    int n = (int)strlen(titulo), i;
    int libre = ANCHO_PANEL - 2 - n;
    if (libre < 0) libre = 0;
    lineaPanel();
    margenPanel(); putchar('|');
    for (i = 0; i < libre / 2; i++) putchar(' ');
    printf("%s", titulo);
    for (i = 0; i < libre - libre / 2; i++) putchar(' ');
    puts("|");
    lineaPanel();
}

void opcionPanel(int numero, const char *descripcion) {
    char linea[128];
    snprintf(linea, sizeof(linea), "  [%2d]  %s", numero, descripcion);
    textoPanel(linea);
}

void encabezadoSistema(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    puts("");
    tituloPanel("TIENDA_PRO");
    margenPanel(); puts("            SISTEMA DE GESTION DE INDUMENTARIA");
    puts("");
}


int buscarCategoria(Categoria categorias[], int cantCategorias, int id) {
    int i;

    for (i = 0; i < cantCategorias; i++) {
        if (categorias[i].id == id) {
            return i;
        }
    }

    return -1;
}

int buscarProveedor(Proveedor proveedores[], int cantProv, int id) {
    int i;

    for (i = 0; i < cantProv; i++) {
        if (proveedores[i].id == id) {
            return i;
        }
    }

    return -1;
}

int buscarPrenda(Prenda prendas[], int cantPrendas, char codigo[]) {
    int i;

    for (i = 0; i < cantPrendas; i++) {
        if (strcmp(prendas[i].codigo, codigo) == 0) {
            return i;
        }
    }

    return -1;
}

void mostrarCategorias(Categoria categorias[], int cantCategorias) {
    int i;

    printf("\n  CATEGORIAS DISPONIBLES:\n");
    printf("  ------------------------------------------------------------------\n");

    for (i = 0; i < cantCategorias; i++) {
        printf("  ID: %d | %-20s | Recargo: %.2f%%\n",
               categorias[i].id,
               categorias[i].nombre,
               categorias[i].porcentajeRecargo);
    }

    printf("  ------------------------------------------------------------------\n");
}


/* ============================================================================
   DATOS INICIALES
   ============================================================================ */

void cargarDatosIniciales(
    Usuario usuarios[],
    int cantUsuarios[],
    Categoria categorias[],
    int cantCategorias[],
    Proveedor proveedores[],
    int cantProv[],
    Prenda prendas[],
    int cantPrendas[]
) {
    /* -------------------- USUARIOS -------------------- */

    usuarios[0] = (Usuario){
        1, "11111111", "Carlos", "Gomez",
        "11-4444-5555", "Av. Corrientes 100", 1, "", ""
    };

    usuarios[1] = (Usuario){
        2, "22222222", "Laura", "Perez",
        "11-6666-7777", "Calle Falsa 123", 2, "", ""
    };

    usuarios[2] = (Usuario){
        3, "44444444", "Maria", "Lopez",
        "11-2222-3333", "San Martin 450", 3, "", ""
    };

    strcpy(usuarios[0].usuario, "admin");
    strcpy(usuarios[0].contrasena, "admin123");
    strcpy(usuarios[1].usuario, "laura");
    strcpy(usuarios[1].contrasena, "repo123");
    strcpy(usuarios[2].usuario, "maria");
    strcpy(usuarios[2].contrasena, "cliente123");
    cantUsuarios[0] = 3;


    /* -------------------- CATEGORIAS -------------------- */

    categorias[0] = (Categoria){1, "Remeras", 20.0};
    categorias[1] = (Categoria){2, "Pantalones", 25.0};
    categorias[2] = (Categoria){3, "Camperas", 30.0};
    categorias[3] = (Categoria){4, "Sacos", 30.0};

    cantCategorias[0] = 4;


    /* -------------------- PROVEEDORES -------------------- */

    proveedores[0].id = 101;
    strcpy(proveedores[0].razonSocial, "Textil Sur SRL");
    proveedores[0].origen = 1;
    proveedores[0].categorias[0] = 1;
    proveedores[0].categorias[1] = 2;
    proveedores[0].cantidadCategorias = 2;

    proveedores[1].id = 102;
    strcpy(proveedores[1].razonSocial, "Importadora Global");
    proveedores[1].origen = 2;
    proveedores[1].categorias[0] = 3;
    proveedores[1].categorias[1] = 4;
    proveedores[1].cantidadCategorias = 2;

    strcpy(proveedores[0].usuario, "prov101");
    strcpy(proveedores[0].contrasena, "textil123");
    strcpy(proveedores[1].usuario, "prov102");
    strcpy(proveedores[1].contrasena, "import123");
    cantProv[0] = 2;


    /* -------------------- PRENDAS -------------------- */

    strcpy(prendas[0].codigo, "RE01");
    strcpy(prendas[0].nombre, "Remera Oversize");
    prendas[0].idCategoria = 1;
    prendas[0].costoCompra = 12500.0;
    prendas[0].precioVenta = 15000.0;
    prendas[0].stockS = 2;
    prendas[0].stockM = 5;
    prendas[0].stockL = 10;

    strcpy(prendas[1].codigo, "PA01");
    strcpy(prendas[1].nombre, "Pantalon Cargo");
    prendas[1].idCategoria = 2;
    prendas[1].costoCompra = 25600.0;
    prendas[1].precioVenta = 32000.0;
    prendas[1].stockS = 8;
    prendas[1].stockM = 12;
    prendas[1].stockL = 4;

    cantPrendas[0] = 2;
}


/* ============================================================================
   USUARIOS
   ============================================================================ */

void registrarUsuario(Usuario usuarios[], int cantUsuarios[]) {
    Usuario u;

    if (cantUsuarios[0] >= 100) {
        printf("\n  [!] Limite maximo de usuarios alcanzado.\n");
        pausarLimpiar();
        return;
    }

    encabezadoSistema();

    printf("                    [ ALTA DE USUARIO ]\n");
    printf("--------------------------------------------------------------------\n");

    /*
       El ID es global y autoincremental.
       Ejemplo: 1, 2, 3, 4...
    */
    u.id = cantUsuarios[0] + 1;

    printf("  ID asignado: %d\n", u.id);

    printf("  > DNI: ");
    scanf("%s", u.dni);

    printf("  > Nombre: ");
    scanf("%s", u.nombre);

    printf("  > Apellido: ");
    scanf("%s", u.apellido);

    printf("  > Telefono: ");
    scanf("%s", u.telefono);

    printf("  > Direccion: ");
    scanf(" %[^\n]", u.direccion);

    do {
        printf("\n  Seleccione el perfil:\n");
        printf("    1. Administrador\n");
        printf("    2. Repositor\n");
        printf("    3. Cliente\n");
        printf("  > Opcion: ");
        scanf("%d", &u.rol);

        if (u.rol < 1 || u.rol > 3) {
            printf("  [!] Opcion invalida.\n");
        }

    } while (u.rol < 1 || u.rol > 3);

    printf("  > Nombre de usuario: ");
    scanf("%29s", u.usuario);
    printf("  > Contrasena: ");
    scanf("%29s", u.contrasena);

    usuarios[cantUsuarios[0]] = u;
    cantUsuarios[0]++;

    printf("\n  [OK] Usuario registrado correctamente.\n");

    pausarLimpiar();
}

void menuCliente(Prenda prendas[], int cantPrendas, Categoria categorias[],
                 int cantCategorias, Caja caja[]);

/* ============================================================================
   AUTENTICACION Y REGISTRO DE CLIENTES
   ============================================================================ */

int autenticarUsuario(Usuario usuarios[], int cantUsuarios, int rolEsperado) {
    char nombreUsuario[30];
    char contrasena[30];
    int i;

    printf("\n  Nombre de usuario: ");
    scanf("%29s", nombreUsuario);
    printf("  Contrasena: ");
    scanf("%29s", contrasena);

    for (i = 0; i < cantUsuarios; i++) {
        if (strcmp(usuarios[i].usuario, nombreUsuario) == 0 &&
            strcmp(usuarios[i].contrasena, contrasena) == 0 &&
            usuarios[i].rol == rolEsperado) {
            printf("\n  [OK] Bienvenido/a %s %s.\n",
                   usuarios[i].nombre, usuarios[i].apellido);
            return i;
        }
    }

    printf("\n  [!] Usuario, contrasena o perfil incorrecto.\n");
    pausarLimpiar();
    return -1;
}

int registrarCliente(Usuario usuarios[], int cantUsuarios[]) {
    Usuario u;
    int i;

    if (cantUsuarios[0] >= 100) {
        printf("\n  [!] No se pueden registrar mas usuarios.\n");
        pausarLimpiar();
        return -1;
    }

    encabezadoSistema();
    printf("                    [ REGISTRO DE CLIENTE ]\n");
    printf("--------------------------------------------------------------------\n");

    u.id = 1;
    for (i = 0; i < cantUsuarios[0]; i++) {
        if (usuarios[i].id >= u.id) u.id = usuarios[i].id + 1;
    }
    u.rol = 3;

    printf("  > DNI: "); scanf("%14s", u.dni);
    printf("  > Nombre: "); scanf("%49s", u.nombre);
    printf("  > Apellido: "); scanf("%49s", u.apellido);
    printf("  > Telefono: "); scanf("%19s", u.telefono);
    printf("  > Direccion: "); scanf(" %99[^\n]", u.direccion);

    do {
        int repetido = 0;
        printf("  > Crear nombre de usuario: ");
        scanf("%29s", u.usuario);
        for (i = 0; i < cantUsuarios[0]; i++) {
            if (strcmp(usuarios[i].usuario, u.usuario) == 0) {
                repetido = 1;
                break;
            }
        }
        if (repetido) {
            printf("  [!] Ese nombre de usuario ya existe. Elegi otro.\n");
        } else {
            break;
        }
    } while (1);

    printf("  > Crear contrasena: ");
    scanf("%29s", u.contrasena);

    usuarios[cantUsuarios[0]] = u;
    cantUsuarios[0]++;
    printf("\n  [OK] Cuenta de cliente creada correctamente.\n");
    printf("  Tu ID de cliente es: %d\n", u.id);
    pausarLimpiar();
    return cantUsuarios[0] - 1;
}

void accesoCliente(Usuario usuarios[], int cantUsuarios[],
                   Prenda prendas[], int cantPrendas,
                   Categoria categorias[], int cantCategorias,
                   Caja caja[]) {
    int opcion, pos;
    encabezadoSistema();
    printf("                    [ ACCESO DE CLIENTES ]\n");
    printf("--------------------------------------------------------------------\n");
    printf("  1. Iniciar sesion\n");
    printf("  2. Registrarme como cliente\n");
    printf("  3. Volver al menu principal\n");
    printf("  > Opcion: ");
    scanf("%d", &opcion);

    if (opcion == 1) {
        pos = autenticarUsuario(usuarios, cantUsuarios[0], 3);
        if (pos != -1)
            menuCliente(prendas, cantPrendas, categorias, cantCategorias, caja);
    } else if (opcion == 2) {
        pos = registrarCliente(usuarios, cantUsuarios);
        if (pos != -1)
            menuCliente(prendas, cantPrendas, categorias, cantCategorias, caja);
    } else if (opcion != 3) {
        printf("\n  [!] Opcion invalida.\n");
        pausarLimpiar();
    }
}

void listarUsuarios(Usuario usuarios[], int cantUsuarios) {
    int i;

    encabezadoSistema();

    printf("                    [ LISTADO DE USUARIOS ]\n");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < cantUsuarios; i++) {

        printf("\n  ID: %d\n", usuarios[i].id);
        printf("  Nombre: %s %s\n",
               usuarios[i].nombre,
               usuarios[i].apellido);
        printf("  DNI: %s\n", usuarios[i].dni);
        printf("  Telefono: %s\n", usuarios[i].telefono);

        switch (usuarios[i].rol) {
            case 1:
                printf("  Rol: Administrador\n");
                break;

            case 2:
                printf("  Rol: Repositor\n");
                break;

            case 3:
                printf("  Rol: Cliente\n");
                break;
        }

        printf("  ------------------------------------------------------------------\n");
    }

    pausarLimpiar();
}


/* ============================================================================
   CATEGORIAS
   ============================================================================ */

void registrarCategoria(Categoria categorias[], int cantCategorias[]) {
    Categoria c;

    if (cantCategorias[0] >= 30) {
        printf("\n  [!] Limite de categorias alcanzado.\n");
        pausarLimpiar();
        return;
    }

    encabezadoSistema();

    printf("                    [ NUEVA CATEGORIA ]\n");
    printf("--------------------------------------------------------------------\n");

    c.id = cantCategorias[0] + 1;

    printf("  ID asignado: %d\n", c.id);

    printf("  > Nombre de la categoria: ");
    scanf(" %[^\n]", c.nombre);

    do {
        printf("  > Porcentaje de recargo sobre el costo: ");
        scanf("%f", &c.porcentajeRecargo);

        if (c.porcentajeRecargo < 0) {
            printf("  [!] El porcentaje no puede ser negativo.\n");
        }

    } while (c.porcentajeRecargo < 0);

    categorias[cantCategorias[0]] = c;
    cantCategorias[0]++;

    printf("\n  [OK] Categoria registrada.\n");
    printf("  El precio de venta se calculara usando este recargo.\n");

    pausarLimpiar();
}

void modificarRecargoCategoria(
    Categoria categorias[],
    int cantCategorias
) {
    int id;
    int pos;
    float nuevoPorcentaje;

    encabezadoSistema();

    printf("                [ MODIFICAR RECARGO POR CATEGORIA ]\n");
    printf("--------------------------------------------------------------------\n");

    mostrarCategorias(categorias, cantCategorias);

    printf("\n  > Ingrese ID de categoria: ");
    scanf("%d", &id);

    pos = buscarCategoria(categorias, cantCategorias, id);

    if (pos == -1) {
        printf("\n  [!] Categoria no encontrada.\n");
        pausarLimpiar();
        return;
    }

    printf("\n  Categoria: %s\n", categorias[pos].nombre);
    printf("  Recargo actual: %.2f%%\n", categorias[pos].porcentajeRecargo);

    do {
        printf("  > Nuevo porcentaje: ");
        scanf("%f", &nuevoPorcentaje);

        if (nuevoPorcentaje < 0) {
            printf("  [!] El porcentaje no puede ser negativo.\n");
        }

    } while (nuevoPorcentaje < 0);

    categorias[pos].porcentajeRecargo = nuevoPorcentaje;

    printf("\n  [OK] Recargo actualizado.\n");
    printf("  IMPORTANTE: esto no modifica automaticamente el stock existente.\n");

    pausarLimpiar();
}


/* ============================================================================
   PROVEEDORES
   ============================================================================ */

void mostrarProveedores(
    Proveedor proveedores[],
    int cantProv,
    Categoria categorias[],
    int cantCategorias
) {
    int i;
    int j;
    int posCategoria;

    encabezadoSistema();

    printf("                    [ LISTADO DE PROVEEDORES ]\n");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < cantProv; i++) {

        printf("\n  ID: %d\n", proveedores[i].id);
        printf("  Razon social: %s\n", proveedores[i].razonSocial);

        if (proveedores[i].origen == 1) {
            printf("  Origen: Nacional\n");
        } else {
            printf("  Origen: Importado\n");
        }

        printf("  Categorias que comercializa:\n");

        for (j = 0; j < proveedores[i].cantidadCategorias; j++) {

            posCategoria = buscarCategoria(
                categorias,
                cantCategorias,
                proveedores[i].categorias[j]
            );

            if (posCategoria != -1) {
                printf("     - %s\n", categorias[posCategoria].nombre);
            }
        }

        printf("  ------------------------------------------------------------------\n");
    }

    pausarLimpiar();
}

void registrarProveedor(
    Proveedor proveedores[],
    int cantProv[],
    Categoria categorias[],
    int cantCategorias
) {
    Proveedor p;
    int opcionOrigen;
    int idCategoria;
    int posCategoria;

    if (cantProv[0] >= 50) {
        printf("\n  [!] Limite maximo de proveedores alcanzado.\n");
        pausarLimpiar();
        return;
    }

    encabezadoSistema();

    printf("                    [ NUEVO PROVEEDOR ]\n");
    printf("--------------------------------------------------------------------\n");

    p.id = 100 + cantProv[0] + 1;

    printf("  ID asignado: %d\n", p.id);

    printf("  > Razon social / Empresa: ");
    scanf(" %[^\n]", p.razonSocial);

    do {
        printf("\n  Origen:\n");
        printf("    1. Nacional\n");
        printf("    2. Importado\n");
        printf("  > Opcion: ");
        scanf("%d", &opcionOrigen);

    } while (opcionOrigen < 1 || opcionOrigen > 2);

    p.origen = opcionOrigen;
    p.cantidadCategorias = 0;

    while (p.cantidadCategorias < 10) {

        encabezadoSistema();

        printf("Proveedor: %s\n", p.razonSocial);
        printf("Categorias asignadas: %d\n\n", p.cantidadCategorias);

        mostrarCategorias(categorias, cantCategorias);

        printf("\n  Ingrese ID de categoria.\n");
        printf("  Ingrese 0 para finalizar.\n");
        printf("  > ID: ");
        scanf("%d", &idCategoria);

        if (idCategoria == 0) {
            break;
        }

        posCategoria = buscarCategoria(
            categorias,
            cantCategorias,
            idCategoria
        );

        if (posCategoria == -1) {
            printf("\n  [!] La categoria no existe.\n");
            pausarLimpiar();
        } else {
            proveedores[cantProv[0]].categorias[p.cantidadCategorias]
                = idCategoria;

            p.categorias[p.cantidadCategorias] = idCategoria;
            p.cantidadCategorias++;

            printf("\n  [OK] Categoria agregada al proveedor.\n");
            pausarLimpiar();
        }
    }

    printf("  > Nombre de usuario para el proveedor: ");
    scanf("%29s", p.usuario);
    printf("  > Contrasena del proveedor: ");
    scanf("%29s", p.contrasena);

    proveedores[cantProv[0]] = p;
    cantProv[0]++;

    printf("\n  [OK] Proveedor registrado correctamente.\n");

    pausarLimpiar();
}


/* ============================================================================
   PRODUCTOS
   ============================================================================ */

void registrarPrenda(
    Prenda prendas[],
    int cantPrendas[],
    Categoria categorias[],
    int cantCategorias
) {
    Prenda p;
    int idCategoria;
    int posCategoria;

    if (cantPrendas[0] >= 100) {
        printf("\n  [!] Capacidad maxima de productos alcanzada.\n");
        pausarLimpiar();
        return;
    }

    encabezadoSistema();

    printf("                    [ NUEVO PRODUCTO ]\n");
    printf("--------------------------------------------------------------------\n");

    printf("  > Codigo del producto: ");
    scanf("%s", p.codigo);

    if (buscarPrenda(prendas, cantPrendas[0], p.codigo) != -1) {
        printf("\n  [!] Ese codigo ya existe.\n");
        pausarLimpiar();
        return;
    }

    printf("  > Nombre / descripcion: ");
    scanf(" %[^\n]", p.nombre);

    mostrarCategorias(categorias, cantCategorias);

    printf("  > ID de categoria: ");
    scanf("%d", &idCategoria);

    posCategoria = buscarCategoria(
        categorias,
        cantCategorias,
        idCategoria
    );

    if (posCategoria == -1) {
        printf("\n  [!] Categoria inexistente.\n");
        pausarLimpiar();
        return;
    }

    p.idCategoria = idCategoria;

    /*
       El producto se crea sin stock.
       La mercaderia entra posteriormente mediante
       "Ingreso de mercaderia".
    */
    p.costoCompra = 0;
    p.precioVenta = 0;
    p.stockS = 0;
    p.stockM = 0;
    p.stockL = 0;

    prendas[cantPrendas[0]] = p;
    cantPrendas[0]++;

    printf("\n  [OK] Producto creado correctamente.\n");
    printf("  Ahora puede ingresar mercaderia para agregar stock y costo.\n");

    pausarLimpiar();
}


/* ============================================================================
   INGRESO DE MERCADERIA
   ============================================================================ */

void ingresarMercaderia(
    Prenda prendas[],
    int cantPrendas,
    Proveedor proveedores[],
    int cantProv,
    Categoria categorias[],
    int cantCategorias
) {
    int idProveedor;
    int posProveedor;

    char codigo[10];
    int posPrenda;

    int idCategoria;
    int categoriaPermitida;
    int i;

    float costo;
    float nuevoPrecio;

    int cantidadS;
    int cantidadM;
    int cantidadL;

    encabezadoSistema();

    printf("                    [ INGRESO DE MERCADERIA ]\n");
    printf("--------------------------------------------------------------------\n");

    if (cantProv == 0 || cantPrendas == 0) {
        printf("\n  [!] Debe existir al menos un proveedor y un producto.\n");
        pausarLimpiar();
        return;
    }

    /* Paso 1: seleccionar proveedor */

    printf("\n  PROVEEDORES DISPONIBLES:\n");

    for (i = 0; i < cantProv; i++) {
        printf("    %d - %s\n",
               proveedores[i].id,
               proveedores[i].razonSocial);
    }

    printf("\n  > ID del proveedor: ");
    scanf("%d", &idProveedor);

    posProveedor = buscarProveedor(
        proveedores,
        cantProv,
        idProveedor
    );

    if (posProveedor == -1) {
        printf("\n  [!] Proveedor no encontrado.\n");
        pausarLimpiar();
        return;
    }

    /* Paso 2: mostrar categorias del proveedor */

    printf("\n  Categorias que comercializa %s:\n",
           proveedores[posProveedor].razonSocial);

    for (i = 0; i < proveedores[posProveedor].cantidadCategorias; i++) {

        int posCategoria = buscarCategoria(
            categorias,
            cantCategorias,
            proveedores[posProveedor].categorias[i]
        );

        if (posCategoria != -1) {
            printf("    ID %d - %s (Recargo %.2f%%)\n",
                   categorias[posCategoria].id,
                   categorias[posCategoria].nombre,
                   categorias[posCategoria].porcentajeRecargo);
        }
    }

    /* Paso 3: seleccionar categoria */

    printf("\n  > ID de categoria: ");
    scanf("%d", &idCategoria);

    categoriaPermitida = 0;

    for (i = 0; i < proveedores[posProveedor].cantidadCategorias; i++) {

        if (proveedores[posProveedor].categorias[i] == idCategoria) {
            categoriaPermitida = 1;
            break;
        }
    }

    if (!categoriaPermitida) {
        printf("\n  [!] Ese proveedor no comercializa esa categoria.\n");
        pausarLimpiar();
        return;
    }

    /* Paso 4: seleccionar producto */

    printf("\n  PRODUCTOS DE ESA CATEGORIA:\n");

    for (i = 0; i < cantPrendas; i++) {

        if (prendas[i].idCategoria == idCategoria) {
            printf("    %s - %s\n",
                   prendas[i].codigo,
                   prendas[i].nombre);
        }
    }

    printf("\n  > Codigo del producto: ");
    scanf("%s", codigo);

    posPrenda = buscarPrenda(
        prendas,
        cantPrendas,
        codigo
    );

    if (posPrenda == -1) {
        printf("\n  [!] Producto no encontrado.\n");
        pausarLimpiar();
        return;
    }

    if (prendas[posPrenda].idCategoria != idCategoria) {
        printf("\n  [!] El producto no pertenece a la categoria seleccionada.\n");
        pausarLimpiar();
        return;
    }

    /* Paso 5: costo de compra */

    printf("\n  Producto: %s\n", prendas[posPrenda].nombre);

    do {
        printf("  > Costo unitario de compra: $");
        scanf("%f", &costo);

        if (costo <= 0) {
            printf("  [!] El costo debe ser positivo.\n");
        }

    } while (costo <= 0);

    /*
       El precio de venta se calcula automaticamente:
       precioVenta = costo + recargo.
    */

    int posCategoria = buscarCategoria(
        categorias,
        cantCategorias,
        idCategoria
    );

    nuevoPrecio =
        costo * (1 + categorias[posCategoria].porcentajeRecargo / 100);

    /* Paso 6: cantidades */

    do {
        printf("  > Cantidad talle S: ");
        scanf("%d", &cantidadS);

        if (cantidadS < 0) {
            printf("  [!] La cantidad no puede ser negativa.\n");
        }

    } while (cantidadS < 0);

    do {
        printf("  > Cantidad talle M: ");
        scanf("%d", &cantidadM);

        if (cantidadM < 0) {
            printf("  [!] La cantidad no puede ser negativa.\n");
        }

    } while (cantidadM < 0);

    do {
        printf("  > Cantidad talle L: ");
        scanf("%d", &cantidadL);

        if (cantidadL < 0) {
            printf("  [!] La cantidad no puede ser negativa.\n");
        }

    } while (cantidadL < 0);

    /* Actualizar producto */

    prendas[posPrenda].costoCompra = costo;
    prendas[posPrenda].precioVenta = nuevoPrecio;

    prendas[posPrenda].stockS += cantidadS;
    prendas[posPrenda].stockM += cantidadM;
    prendas[posPrenda].stockL += cantidadL;

    printf("\n  ------------------------------------------------------------------\n");
    printf("  [OK] MERCADERIA INGRESADA\n");
    printf("  Proveedor: %s\n", proveedores[posProveedor].razonSocial);
    printf("  Producto: %s\n", prendas[posPrenda].nombre);
    printf("  Costo de compra: $%.2f\n", costo);
    printf("  Recargo de categoria: %.2f%%\n",
           categorias[posCategoria].porcentajeRecargo);
    printf("  Nuevo precio de venta: $%.2f\n", nuevoPrecio);
    printf("  Stock actual: S=%d | M=%d | L=%d\n",
           prendas[posPrenda].stockS,
           prendas[posPrenda].stockM,
           prendas[posPrenda].stockL);
    printf("  ------------------------------------------------------------------\n");

    pausarLimpiar();
}


/* ============================================================================
   STOCK Y CONSULTAS
   ============================================================================ */

void mostrarStock(
    Prenda prendas[],
    int cantPrendas,
    Categoria categorias[],
    int cantCategorias
) {
    int i;
    int posCategoria;

    encabezadoSistema();

    printf("                    [ STOCK GENERAL ]\n");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < cantPrendas; i++) {

        posCategoria = buscarCategoria(
            categorias,
            cantCategorias,
            prendas[i].idCategoria
        );

        printf("\n  Codigo: %s\n", prendas[i].codigo);
        printf("  Producto: %s\n", prendas[i].nombre);

        if (posCategoria != -1) {
            printf("  Categoria: %s\n", categorias[posCategoria].nombre);
        }

        printf("  Costo de compra: $%.2f\n", prendas[i].costoCompra);
        printf("  Precio de venta: $%.2f\n", prendas[i].precioVenta);

        printf("  Stock -> S: %d | M: %d | L: %d\n",
               prendas[i].stockS,
               prendas[i].stockM,
               prendas[i].stockL);

        printf("  ------------------------------------------------------------------\n");
    }

    pausarLimpiar();
}

void consultarRentabilidad(
    Prenda prendas[],
    int cantPrendas,
    Categoria categorias[],
    int cantCategorias
) {
    int i;
    int posCategoria;
    float ganancia;

    encabezadoSistema();

    printf("                [ COSTOS, PRECIOS Y RENTABILIDAD ]\n");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < cantPrendas; i++) {

        posCategoria = buscarCategoria(
            categorias,
            cantCategorias,
            prendas[i].idCategoria
        );

        ganancia =
            prendas[i].precioVenta - prendas[i].costoCompra;

        printf("\n  Producto: %s (%s)\n",
               prendas[i].nombre,
               prendas[i].codigo);

        if (posCategoria != -1) {
            printf("  Categoria: %s\n",
                   categorias[posCategoria].nombre);

            printf("  Recargo aplicado: %.2f%%\n",
                   categorias[posCategoria].porcentajeRecargo);
        }

        printf("  Costo de compra: $%.2f\n",
               prendas[i].costoCompra);

        printf("  Precio de venta: $%.2f\n",
               prendas[i].precioVenta);

        printf("  Ganancia por unidad: $%.2f\n",
               ganancia);

        printf("  ------------------------------------------------------------------\n");
    }

    pausarLimpiar();
}

void alertaStockCritico(
    Prenda prendas[],
    int cantPrendas
) {
    int i;
    int hayAlerta = 0;

    encabezadoSistema();

    printf("                  [ ALERTAS DE STOCK CRITICO ]\n");
    printf("--------------------------------------------------------------------\n");

    for (i = 0; i < cantPrendas; i++) {

        if (prendas[i].stockS < 3 ||
            prendas[i].stockM < 3 ||
            prendas[i].stockL < 3) {

            printf("\n  [!] REPOSICION REQUERIDA\n");
            printf("  Producto: %s (%s)\n",
                   prendas[i].nombre,
                   prendas[i].codigo);

            printf("  Stock -> S: %d | M: %d | L: %d\n",
                   prendas[i].stockS,
                   prendas[i].stockM,
                   prendas[i].stockL);

            printf("  ------------------------------------------------------------------\n");

            hayAlerta = 1;
        }
    }

    if (!hayAlerta) {
        printf("\n  [OK] No hay productos con stock critico.\n");
    }

    pausarLimpiar();
}


/* ============================================================================
   VENTAS Y FACTURACION
   ============================================================================ */

void emitirFactura(
    char tipoFac,
    char cliente[],
    char producto[],
    char talle,
    int cantidad,
    float total,
    char medioPago[]
) {
    encabezadoSistema();

    printf("                         FACTURA %c\n", toupper(tipoFac));
    printf("--------------------------------------------------------------------\n");

    printf("  Cliente: %s\n", cliente);
    printf("  Producto: %s\n", producto);
    printf("  Talle: %c\n", toupper(talle));
    printf("  Cantidad: %d\n", cantidad);
    printf("  Medio de pago: %s\n", medioPago);

    printf("  ------------------------------------------------------------------\n");
    printf("  TOTAL: $%.2f\n", total);
    printf("  ------------------------------------------------------------------\n");

    printf("                    Gracias por su compra.\n");
}

void realizarVenta(
    Prenda prendas[],
    int cantPrendas,
    Caja caja[]
) {
    char codigo[10];
    char talle;
    char cliente[50];
    char tipoFac;
    char medioStr[50];

    int pos;
    int stockDisponible;
    int cantidad;
    int medioPago;

    float subtotal;
    float totalFinal;

    encabezadoSistema();

    printf("                       [ MODULO DE VENTAS ]\n");
    printf("--------------------------------------------------------------------\n");

    printf("  > Codigo del producto: ");
    scanf("%s", codigo);

    pos = buscarPrenda(
        prendas,
        cantPrendas,
        codigo
    );

    if (pos == -1) {
        printf("\n  [!] Producto no encontrado.\n");
        pausarLimpiar();
        return;
    }

    printf("\n  Producto: %s\n", prendas[pos].nombre);
    printf("  Precio de venta: $%.2f\n", prendas[pos].precioVenta);

    printf("\n  > Talle (S/M/L): ");
    scanf(" %c", &talle);

    talle = toupper(talle);

    if (talle == 'S') {
        stockDisponible = prendas[pos].stockS;
    } else if (talle == 'M') {
        stockDisponible = prendas[pos].stockM;
    } else if (talle == 'L') {
        stockDisponible = prendas[pos].stockL;
    } else {
        printf("\n  [!] Talle invalido.\n");
        pausarLimpiar();
        return;
    }

    printf("  Stock disponible: %d\n", stockDisponible);

    printf("  > Cantidad: ");
    scanf("%d", &cantidad);

    if (cantidad <= 0 || cantidad > stockDisponible) {
        printf("\n  [!] Cantidad invalida o stock insuficiente.\n");
        pausarLimpiar();
        return;
    }

    printf("  > Nombre del cliente: ");
    scanf(" %[^\n]", cliente);

    printf("  > Tipo de comprobante (A/B/C): ");
    scanf(" %c", &tipoFac);

    printf("\n  Medio de pago:\n");
    printf("    1. Efectivo - 10%% de descuento\n");
    printf("    2. Tarjeta / Transferencia\n");
    printf("  > Opcion: ");
    scanf("%d", &medioPago);

    if (medioPago != 1 && medioPago != 2) {
        printf("\n  [!] Medio de pago invalido.\n");
        pausarLimpiar();
        return;
    }

    subtotal = prendas[pos].precioVenta * cantidad;
    totalFinal = subtotal;

    if (medioPago == 1) {
        totalFinal = subtotal * 0.90;
        strcpy(medioStr, "Efectivo - 10% descuento");

        caja[0].cajaEfectivo += totalFinal;
    } else {
        strcpy(medioStr, "Tarjeta / Transferencia");

        caja[0].cajaTarjeta += totalFinal;
    }

    /* Descontar stock */

    if (talle == 'S') {
        prendas[pos].stockS -= cantidad;
    } else if (talle == 'M') {
        prendas[pos].stockM -= cantidad;
    } else {
        prendas[pos].stockL -= cantidad;
    }

    /* Actualizar caja */

    caja[0].cajaTotal += totalFinal;
    caja[0].totalFacturas++;

    emitirFactura(
        tipoFac,
        cliente,
        prendas[pos].nombre,
        talle,
        cantidad,
        totalFinal,
        medioStr
    );

    registrarVentaArchivo(tipoFac, cliente, prendas[pos].codigo, prendas[pos].nombre,
                          talle, cantidad, totalFinal, medioStr);

    pausarLimpiar();
}


/* ============================================================================
   CAJA
   ============================================================================ */

void cierreCajaZ(Caja caja[]) {

    encabezadoSistema();

    printf("                    [ CIERRE DE CAJA Z ]\n");
    printf("--------------------------------------------------------------------\n");

    printf("  Facturas emitidas: %d\n",
           caja[0].totalFacturas);

    printf("  Efectivo: $%.2f\n",
           caja[0].cajaEfectivo);

    printf("  Tarjeta / Transferencia: $%.2f\n",
           caja[0].cajaTarjeta);

    printf("  ------------------------------------------------------------------\n");

    printf("  TOTAL RECAUDADO: $%.2f\n",
           caja[0].cajaTotal);

    printf("  ------------------------------------------------------------------\n");

    /*
       Se reinicia la caja luego del cierre.
    */

    caja[0].cajaTotal = 0;
    caja[0].cajaEfectivo = 0;
    caja[0].cajaTarjeta = 0;
    caja[0].totalFacturas = 0;

    printf("  [OK] Caja reiniciada para un nuevo turno.\n");

    pausarLimpiar();
}


/* ============================================================================
   MENU ADMINISTRADOR
   ============================================================================ */

void menuAdmin(
    Usuario usuarios[],
    int cantUsuarios[],
    Categoria categorias[],
    int cantCategorias[],
    Proveedor proveedores[],
    int cantProv[],
    Prenda prendas[],
    int cantPrendas[],
    Caja caja[]
) {
    int opc;

    do {

        encabezadoSistema();
        tituloPanel("PANEL ADMINISTRADOR");
        textoPanel("");
        opcionPanel(1, "Registrar un nuevo usuario");
        opcionPanel(2, "Consultar usuarios registrados");
        opcionPanel(3, "Registrar una nueva categoria de productos");
        opcionPanel(4, "Modificar recargo de una categoria");
        opcionPanel(5, "Registrar un nuevo proveedor");
        opcionPanel(6, "Crear un nuevo producto");
        opcionPanel(7, "Ingresar mercaderia y actualizar stock");
        opcionPanel(8, "Consultar costos, precios y ganancias");
        opcionPanel(9, "Realizar cierre de caja Z");
        opcionPanel(10, "Cerrar sesion");
        textoPanel("");
        lineaPanel();
        puts("");
        margenPanel(); printf("Seleccione una opcion: ");
        scanf("%d", &opc);

        switch (opc) {

            case 1:
                registrarUsuario(
                    usuarios,
                    cantUsuarios
                );
                break;

            case 2:
                listarUsuarios(
                    usuarios,
                    cantUsuarios[0]
                );
                break;

            case 3:
                registrarCategoria(
                    categorias,
                    cantCategorias
                );
                break;

            case 4:
                modificarRecargoCategoria(
                    categorias,
                    cantCategorias[0]
                );
                break;

            case 5:
                registrarProveedor(
                    proveedores,
                    cantProv,
                    categorias,
                    cantCategorias[0]
                );
                break;

            case 6:
                registrarPrenda(
                    prendas,
                    cantPrendas,
                    categorias,
                    cantCategorias[0]
                );
                break;

            case 7:
                ingresarMercaderia(
                    prendas,
                    cantPrendas[0],
                    proveedores,
                    cantProv[0],
                    categorias,
                    cantCategorias[0]
                );
                break;

            case 8:
                consultarRentabilidad(
                    prendas,
                    cantPrendas[0],
                    categorias,
                    cantCategorias[0]
                );
                break;

            case 9:
                cierreCajaZ(caja);
                break;

            case 10:
                break;

            default:
                printf("\n  [!] Opcion invalida.\n");
                pausarLimpiar();
        }

    } while (opc != 10);
}


/* ============================================================================
   MENU REPOSITOR
   ============================================================================ */

void menuRepositor(
    Prenda prendas[],
    int cantPrendas,
    Categoria categorias[],
    int cantCategorias,
    Proveedor proveedores[],
    int cantProv
) {
    int opc;

    do {

        encabezadoSistema();
        tituloPanel("PANEL REPOSITOR");
        textoPanel("");
        opcionPanel(1, "Consultar stock actual");
        opcionPanel(2, "Consultar alertas de stock critico");
        opcionPanel(3, "Ingresar mercaderia");
        opcionPanel(4, "Volver al menu principal");
        textoPanel("");
        lineaPanel();
        puts("");
        margenPanel(); printf("Seleccione una opcion: ");
        scanf("%d", &opc);

        switch (opc) {

            case 1:
                mostrarStock(
                    prendas,
                    cantPrendas,
                    categorias,
                    cantCategorias
                );
                break;

            case 2:
                alertaStockCritico(
                    prendas,
                    cantPrendas
                );
                break;

            case 3:
                ingresarMercaderia(
                    prendas,
                    cantPrendas,
                    proveedores,
                    cantProv,
                    categorias,
                    cantCategorias
                );
                break;

            case 4:
                break;

            default:
                printf("\n  [!] Opcion invalida.\n");
                pausarLimpiar();
        }

    } while (opc != 4);
}


/* ============================================================================
   MENU CLIENTE
   ============================================================================ */

void menuCliente(
    Prenda prendas[],
    int cantPrendas,
    Categoria categorias[],
    int cantCategorias,
    Caja caja[]
) {
    int opc;

    do {

        encabezadoSistema();
        tituloPanel("PANEL CLIENTE");
        textoPanel("");
        opcionPanel(1, "Ver catalogo, stock y precios");
        opcionPanel(2, "Comprar producto y emitir comprobante");
        opcionPanel(3, "Volver al menu principal");
        textoPanel("");
        lineaPanel();
        puts("");
        margenPanel(); printf("Seleccione una opcion: ");
        scanf("%d", &opc);

        switch (opc) {

            case 1:
                mostrarStock(
                    prendas,
                    cantPrendas,
                    categorias,
                    cantCategorias
                );
                break;

            case 2:
                realizarVenta(
                    prendas,
                    cantPrendas,
                    caja
                );
                break;

            case 3:
                break;

            default:
                printf("\n  [!] Opcion invalida.\n");
                pausarLimpiar();
        }

    } while (opc != 3);
}


/* ============================================================================
   MENU PROVEEDOR
   ============================================================================ */

void menuProveedor(
    Proveedor proveedores[],
    int cantProv,
    Categoria categorias[],
    int cantCategorias
) {
    char usuario[30], contrasena[30];
    int i, pos = -1, j;

    encabezadoSistema();
    printf("                     [ ACCESO DE PROVEEDORES ]\n");
    printf("--------------------------------------------------------------------\n");
    printf("  > Nombre de usuario: ");
    scanf("%29s", usuario);
    printf("  > Contrasena: ");
    scanf("%29s", contrasena);

    for (i = 0; i < cantProv; i++) {
        if (strcmp(proveedores[i].usuario, usuario) == 0 &&
            strcmp(proveedores[i].contrasena, contrasena) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("\n  [!] Usuario o contrasena de proveedor incorrectos.\n");
        pausarLimpiar();
        return;
    }

    printf("\n  [OK] Acceso autorizado.\n");
    printf("  Proveedor: %s\n", proveedores[pos].razonSocial);
    printf("  ID: %d\n", proveedores[pos].id);
    printf("  Origen: %s\n", proveedores[pos].origen == 1 ? "Nacional" : "Importado");
    printf("\n  Categorias que puede comercializar:\n");

    for (j = 0; j < proveedores[pos].cantidadCategorias; j++) {
        int posCategoria = buscarCategoria(categorias, cantCategorias,
                                           proveedores[pos].categorias[j]);
        if (posCategoria != -1)
            printf("    - %s\n", categorias[posCategoria].nombre);
    }
    pausarLimpiar();
}

/* ============================================================================
   MAIN
   ============================================================================ */

int main() {

    Usuario usuarios[100];
    Categoria categorias[30];
    Proveedor proveedores[50];
    Prenda prendas[100];
    Caja caja[1];

    int cantUsuarios[1] = {0};
    int cantCategorias[1] = {0};
    int cantProv[1] = {0};
    int cantPrendas[1] = {0};

    /* Primero se preparan los datos de ejemplo. Si hay archivos, se leen encima;
       si no existen, los datos iniciales se guardan para la primera ejecucion. */
    cargarDatosIniciales(usuarios, cantUsuarios, categorias, cantCategorias,
                         proveedores, cantProv, prendas, cantPrendas);

    caja[0].cajaTotal = 0;
    caja[0].cajaEfectivo = 0;
    caja[0].cajaTarjeta = 0;
    caja[0].totalFacturas = 0;

    {
        int n;
        n = cargarUsuarios(usuarios, 100);
        if (n > 0) cantUsuarios[0] = n; else guardarUsuarios(usuarios, cantUsuarios[0]);
        n = cargarCategorias(categorias, 30);
        if (n > 0) cantCategorias[0] = n; else guardarCategorias(categorias, cantCategorias[0]);
        n = cargarProveedores(proveedores, 50);
        if (n > 0) cantProv[0] = n; else guardarProveedores(proveedores, cantProv[0]);
        n = cargarPrendas(prendas, 100);
        if (n > 0) cantPrendas[0] = n; else guardarPrendas(prendas, cantPrendas[0]);
        if (!cargarCaja(caja)) guardarCaja(caja);
    }

    int opcion;

    do {

        encabezadoSistema();
        tituloPanel("MENU PRINCIPAL");
        textoPanel("");
        opcionPanel(1, "ADMINISTRADOR");
        opcionPanel(2, "EMPLEADO / REPOSITOR");
        opcionPanel(3, "PROVEEDOR");
        opcionPanel(4, "CLIENTE - Ingresar o registrarse");
        opcionPanel(5, "SALIR DEL SISTEMA");
        textoPanel("");
        lineaPanel();
        puts("");
        margenPanel(); printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {

            case 1:
                encabezadoSistema();
                printf("                 [ INICIO DE SESION ADMINISTRADOR ]\n");
                if (autenticarUsuario(usuarios, cantUsuarios[0], 1) != -1) {
                    menuAdmin(usuarios, cantUsuarios, categorias, cantCategorias,
                              proveedores, cantProv, prendas, cantPrendas, caja);
                }
                break;

            case 2:
                encabezadoSistema();
                printf("              [ INICIO DE SESION EMPLEADO / REPOSITOR ]\n");
                if (autenticarUsuario(usuarios, cantUsuarios[0], 2) != -1) {
                    menuRepositor(prendas, cantPrendas[0], categorias,
                                  cantCategorias[0], proveedores, cantProv[0]);
                }
                break;

            case 3:
                menuProveedor(proveedores, cantProv[0], categorias, cantCategorias[0]);
                break;

            case 4:
                accesoCliente(usuarios, cantUsuarios, prendas, cantPrendas[0],
                              categorias, cantCategorias[0], caja);
                break;

            case 5:
                encabezadoSistema();
                printf("\n  [OK] Sistema finalizado correctamente.\n\n");
                break;

            default:
                printf("\n  [!] Opcion invalida.\n");
                pausarLimpiar();
        }

        /* Al volver de cualquier menu, se persisten todos los cambios. */
        guardarTodosLosDatos(usuarios, cantUsuarios[0], categorias, cantCategorias[0],
                             proveedores, cantProv[0], prendas, cantPrendas[0], caja);

    } while (opcion != 5);

    guardarTodosLosDatos(usuarios, cantUsuarios[0], categorias, cantCategorias[0],
                         proveedores, cantProv[0], prendas, cantPrendas[0], caja);
    return 0;
}
