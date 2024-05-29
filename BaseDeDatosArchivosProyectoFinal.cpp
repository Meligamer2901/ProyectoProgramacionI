// James Emmanuel Bojorquez Gutierrez 5090-23-1345
// Roberto Carlos de León Gramajo 5090-23-1762
// Angelyn Judith Díaz Zeceña 5090-23-1407
// Katherine Melissa Palacios Salguero 5090-23-682
// Karla Rossmery Ramírez Ruano 5090-23-1802
#include <iostream>
#include <string>
#include <vector> 
#include <mysql.h>
#include <iomanip> 
#include <algorithm>
using namespace std;

class MySQLConnector {
private:
    MYSQL* connection;
    MYSQL_RES* res;
    MYSQL_STMT* stmt;

public:
// Esta parte del codigo nos permite entrar a la base de datos, si consigue entrar nos indicara conexion exitosa y luego el menu plegable
// En dado caso no conecte nos indicara error en la conexion y fin del programa
    MySQLConnector(const char* host, const char* user, const char* password, const char* database, int port) {
        connection = mysql_init(0);
        connection = mysql_real_connect(connection, host, user, password, database, port, NULL, 0);
        if (connection) {
            cout << "Conexion Exitosa..." << endl;
        }
        else {
            cout << "Error en la Conexion...";
        }
    }

//Void el que nos permite ingresar datos
    void insert(const string& query) {
        executeQuery(query);
    }

//Void el que permite actualizar datos
    void update(const string& query) {
        executeQuery(query);
    }

//Void el que permite eliminar datos datos
    void del(const string& query) {
        executeQuery(query);
    }

    bool select(const string& query) {
        if (mysql_query(connection, query.c_str()) == 0) {
            res = mysql_store_result(connection);
            if (res) {
                int num_fields = mysql_num_fields(res);
                MYSQL_FIELD* fields = mysql_fetch_fields(res);
                vector<size_t> max_lengths(num_fields, 0); // Longitud máxima de cada campo

                // Calcular la longitud máxima de cada campo
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res)) != NULL) {
                    for (int i = 0; i < num_fields; i++) {
                        if (row[i] && strlen(row[i]) > max_lengths[i]) {
                            max_lengths[i] = strlen(row[i]);
                        }
                    }
                }

                // Imprimir los datos con alineación
                mysql_data_seek(res, 0); // Reiniciar el cursor
                while ((row = mysql_fetch_row(res)) != NULL) {
                    for (int i = 0; i < num_fields; i++) {
                        cout << left << setw(max_lengths[i] + 2) << row[i];
                    }
                    cout << endl;
                }

                return true; // Se encontraron resultados
            }
        }
        else {
            cout << "Error al ejecutar el query: " << mysql_error(connection) << endl;
        }

        return false; // No se encontraron resultados o hubo un error
    }


    ~MySQLConnector() {
        mysql_free_result(res);
        mysql_close(connection);
    }

private:
    void executeQuery(const string& query) {
        if (mysql_query(connection, query.c_str()) == 0) {
            cout << "Query ejecutado correctamente." << endl;
        }
        else {
            cout << "Error al ejecutar el query: " << mysql_error(connection) << endl;
        }
    }
};

class Cliente {
private:
    string nit_cliente;
    string nombre;
    string apellido;
    string email;
    string telefono;

class ClientesCumpleaños : public Cliente {
private:
    string fecha_cumpleaños;
    int edad;

public:
    Cliente(const string& nit, const string& nom, const string& ape, const string& mail, const string& tel)
        : nit_cliente(nit), nombre(nom), apellido(ape), email(mail), telefono(tel) {}

    // Getters y Setters
    string getNitCliente() const { return nit_cliente; }
    void setNitCliente(const string& nit) { nit_cliente = nit; }

    string getNombre() const { return nombre; }
    void setNombre(const string& nom) { nombre = nom; }

    string getApellido() const { return apellido; }
    void setApellido(const string& ape) { apellido = ape; }

    string getEmail() const { return email; }
    void setEmail(const string& mail) { email = mail; }

    string getTelefono() const { return telefono; }
    void setTelefono(const string& tel) { telefono = tel; }

    // Funciones para actualizar e insertar datos en la base de datos
    void updateCliente(MySQLConnector& connector, const string& tabla) {
        string campo;
        string valor;
        string id;
        cout << "Ingrese el NIT del cliente que desea modificar: ";
        cin >> id;
        cout << "Seleccione el campo que desea modificar:\n1. NIT del cliente\n2. Nombre del cliente\n3. Apellido del cliente\n4. Email\n5. Telefono\nOpcion: ";
        int opcionCampo;
        cin >> opcionCampo;
        switch (opcionCampo) {
        case 1:
            campo = "nit_cliente";
            break;
        case 2:
            campo = "nombre";
            break;
        case 3:
            campo = "apellido";
            break;
        case 4:
            campo = "email";
            break;
        case 5:
            campo = "telefono";
            break;
        default:
            cout << "Opcion de campo invalida." << endl;
            return;
        }
        cout << "Ingrese el nuevo detalle del objeto " << campo << ": ";
        cin.ignore();
        getline(cin, valor);
        string query = "UPDATE " + tabla + " SET " + campo + " = '" + valor + "' WHERE nit_cliente = '" + id + "'";
        connector.update(query);
    }

    void insertarCliente(MySQLConnector& connector, const string& tabla) {
        string nit, nombre, apellido, telefono, email;
        cout << "Ingrese el NIT: ";
        cin.ignore();
        getline(cin, nit);
        cout << "Nombre del cliente: ";
        getline(cin, nombre);
        cout << "Apellido del cliente: ";
        getline(cin, apellido);
        cout << "Email: ";
        getline(cin, email);
        cout << "Numero de telefono: ";
        getline(cin, telefono);

        string query = "INSERT INTO " + tabla + " (nit_cliente, nombre, apellido, email, telefono) VALUES ('" + nit + "','" + nombre + "','" + apellido + "','" + email + "','" + telefono + "')";
        connector.insert(query);
    }

};

public:
    // Constructor
    ClientesCumpleaños(const string& nit, const string& nom, const string& ape, const string& mail, const string& tel)
        : Cliente(nit, nom, ape, mail, tel) {}

    // Setter para la fecha de cumpleaños
    void setFechaCumpleaños(const string& fecha) {
        fecha_cumpleaños = fecha;
    }

    // Setter para la edad
    void setEdad(int years) {
        edad = years;
    }

    // Función para insertar cliente y su información de cumpleaños en el archivo clientesPremium.txt
    void insertarClientePremium(ofstream& archivo) {
        // Insertar información del cliente en el archivo
        archivo << "NIT: " << getNitCliente() << ", Nombre: " << getNombre() << " " << getApellido() << ", Email: " << getEmail() << ", Teléfono: " << getTelefono() << ", Cumpleaños: " << fecha_cumpleaños << ", Edad: " << edad << endl;
    }
};
void insertarClientePremium() {
    // Crear un objeto de la clase ClientesCumpleaños
    ClientesCumpleaños cliente("", "", "", "", "");

    // Solicitar información del cliente
    string nit, nombre, apellido, email, telefono;
    cout << "Ingrese el NIT: ";
    cin.ignore();
    getline(cin, nit);
    cout << "Nombre del cliente: ";
    getline(cin, nombre);
    cout << "Apellido del cliente: ";
    getline(cin, apellido);
    cout << "Email: ";
    getline(cin, email);
    cout << "Numero de telefono: ";
    getline(cin, telefono);

    cliente.setNitCliente(nit);
    cliente.setNombre(nombre);
    cliente.setApellido(apellido);
    cliente.setEmail(email);
    cliente.setTelefono(telefono);

    // Solicitar información del cumpleaños
    string fecha_cumpleaños;
    cout << "Ingrese la fecha de cumpleaños (AAAA-MM-DD): ";
    cin >> fecha_cumpleaños;
    cliente.setFechaCumpleaños(fecha_cumpleaños);

    // Solicitar edad
    int edad;
    cout << "Ingrese la edad: ";
    cin >> edad;
    cliente.setEdad(edad);

    // Abrir archivo clientesPremium.txt en modo de escritura
    ofstream archivo("clientesPremium.txt", ios::app);
    if (archivo.is_open()) {
        // Insertar cliente y su información de cumpleaños en el archivo
        cliente.insertarClientePremium(archivo);
        cout << "Cliente insertado en clientesPremium.txt." << endl;
        archivo.close();
    }
    else {
        cout << "Error al abrir el archivo clientesPremium.txt." << endl;
    }
}

// Prototipos de las funciones
void updateEmpleados(MySQLConnector& connector, const string& tabla);
void updateProductos(MySQLConnector& connector, const string& tabla);
void updateVentas(MySQLConnector& connector, const string& tabla);
void updateDetallesVentas(MySQLConnector& connector, const string& tabla);
void insertarEnEmpleados(MySQLConnector& connector, const string& tabla);
void insertarEnProductos(MySQLConnector& connector, const string& tabla);
void insertarEnVentas(MySQLConnector& connector, const string& tabla);
void insertarEnDetalle_venta(MySQLConnector& connector, const string& tabla);

int main() {
    MySQLConnector connector("localhost", "root", "/BBCN1978*1973", "proyectoProgra", 3306);

    int opcion;
    string query; // Solo declara la variable aquí, sin inicializarla

    do {
        cout << "\nMenu:\n1. Insertar registro\n2. Actualizar registro\n3. Eliminar registro\n4. Mostrar registros\n5. Salir\nOpcion: ";
        cin >> opcion;

        string tabla;
         switch (opcion) {
        case 1://Insertar datos, como todo CRUD necesita datos, y por ellos es importatante poder ingresar datos, según nuestra base de datos ncesitamos empleados, produtos, clientes, ventas, y detalle de ventas
        {
            system("cls");//Utilizamos una limpieza de pantalla, evitando la saturación de información, y siendo más facil de entender para el usuario.
            cout << "Seleccione la tabla en la que desea insertar datos:\n1. Empleados\n2. Productos\n3. Clientes\n4. Venta\n5. Detalles_Venta\nTabla: ";
            int opcionTabla;
            cin >> opcionTabla;//la unica variable que se creara
            //Utilizamos un Switch para brindar la opción de escoger cuál será la tabla a la que se ingresara un nuevo objeto, son 5 opciones y por ellos escribiendo el numero de la tabla que se desea ingresar datos, sera suficiente
            switch (opcionTabla) {//la dato que el usuario ingrese se tomará para el Switch, proporcionando una funcion.
            case 1:
                tabla = "empleados";
                insertarEnEmpleados(connector, tabla);
                break;
            case 2:
                tabla = "productos";
                insertarEnProductos(connector, tabla);
                break;
            case 3: // Clientes
            {
                tabla = "clientes";
                cliente.insertarCliente(connector, tabla);
                break;
            }
            case 4:
                tabla = "ventas";
                insertarEnVentas(connector, tabla);
                break;
            case 5:
                tabla = "detalle_venta";
                insertarEnDetalle_venta(connector, tabla);
                break;
            default://En todo caso que ingrese un número que no es permitido, por ello se definen que numero es de cada tabla
                cout << "Opcion de tabla invalida." << endl;
                continue;
            }
    
            break;
        }
        case 2:
        {
            system("cls");
            cout << "Seleccione la tabla que desea modificar:\n1. Clientes\n2. Detalle_Venta\n3. Empleados\n4. Productos\n5. Ventas\nTabla: ";
            int opcionTabla;
            cin >> opcionTabla;

            switch (opcionTabla) {
            case 1: // Clientes
                tabla = "clientes";
                cliente.updateCliente(connector, tabla);
                break;
            case 2:
                tabla = "detalle_venta";
                updateDetallesVentas(connector, tabla);
                break;
            case 3:
                tabla = "empleados";
                updateEmpleados(connector, tabla);
                break;
            case 4:
                tabla = "productos";
                updateProductos(connector, tabla);
                break;
            case 5:
                tabla = "ventas";
                updateVentas(connector, tabla);
                break;
            default:
                cout << "Opcion de tabla invalida." << endl;
                continue;
            }
            break;
        }
        case 3: {
            system("cls");
            cout << "Seleccione la tabla que contiene el objeto que desea eliminar:\n1. Clientes\n2. Detalle_Venta\n3. Empleados\n4. Productos\n5. Ventas\nTabla: ";
            int opcionTabla;
            cin >> opcionTabla;

            string tabla;
            switch (opcionTabla) {
            case 1:
                tabla = "clientes";
                break;
            case 2:
                tabla = "detalle_venta";
                break;
            case 3:
                tabla = "empleados";
                break;
            case 4:
                tabla = "productos";
                break;
            case 5:
                tabla = "ventas";
                break;
            default:
                cout << "Opcion de tabla invalida." << endl;
                continue;
            }

            string id;
            cout << "Ingrese el ID o NIT del registro que desea eliminar de la tabla " << tabla << ": ";
            cin.ignore();
            getline(cin, id);

            string id_columna;
            if (tabla == "clientes") {
                id_columna = "nit_cliente";
            }
            else if (tabla == "detalle_venta") {
                id_columna = "id_det_venta";
            }
            else if (tabla == "empleados") {
                id_columna = "id_empleado";
            }
            else if (tabla == "productos") {
                id_columna = "id_producto";
            }
            else if (tabla == "ventas") {
                id_columna = "id_venta";
            }

            string query = "SELECT " + id_columna + " FROM " + tabla + " WHERE " + id_columna + " = '" + id + "'";
            if (connector.select(query)) {
                query = "DELETE FROM " + tabla + " WHERE " + id_columna + " = '" + id + "'";
                connector.del(query);
                cout << "Registro eliminado exitosamente." << endl;
            }
            else {
                cout << "El ID especificado no pertenece a la tabla seleccionada." << endl;
            }

            break;
        }

        case 4:
        {
            system("cls");
            cout << "Seleccione la tabla que desea visualizar:\n1. Clientes\n2. Detalle_Venta\n3. Empleados\n4. Productos\n5. Ventas\nTabla: ";
            int opcionTabla;
            cin >> opcionTabla;

            switch (opcionTabla) {
            case 1:
                tabla = "clientes";
                break;
            case 2:
                tabla = "detalle_venta";
                break;
            case 3:
                tabla = "empleados";
                break;
            case 4:
                tabla = "productos";
                break;
            case 5:
                tabla = "ventas";
                break;
            default:
                cout << "Opcion de tabla invalida." << endl;
                continue;
            }

            cout << "Mostrando todos los registros de la tabla " << tabla << ":" << endl;
            query = "SELECT * FROM " + tabla;
            connector.select(query);
            break;
        }
        case 5: // Ver historial
            mostrarHist();
            break;
             
        case 6:
            cout << "Saliendo..." << endl;
            break;
        default:
            cout << "Opcion no valida. Por favor, intente de nuevo." << endl;
        }
    } while (opcion != 6);

    cout << "Presione Enter para salir...";
    cin.ignore();
    cin.get(); // Esperar a que el usuario presione Enter

    return 0;
}



void updateEmpleados(MySQLConnector& connector, const string& tabla) {
    string campo;
    string valor;
    int id;
    cout << "Ingrese el ID del empleado que desea modificar: ";
    cin >> id;
    cout << "Seleccione el campo que desea modificar:\n1. Nombre del empleado\n2. Apellido del empleado\n3. Puesto del trabajador\n4. Ingrese el sexo (Maculino/Femenino)\n5. Salario laboral\nOpcion: ";
    int opcionCampo;
    cin >> opcionCampo;
    switch (opcionCampo) {
    case 1:
        campo = "nombre";
        break;
    case 2:
        campo = "apellido";
        break;
    case 3:
        campo = "puesto";
        break;
    case 4:
        campo = "genero";
        break;
    case 5:
        campo = "salario";
        break;
    default:
        cout << "Opcion de campo invalida." << endl;
        return;
    }
    cout << "Ingrese el nuevo detalle del objeto " << campo << ": ";
    cin.ignore();
    getline(cin, valor);
    string query = "UPDATE " + tabla + " SET " + campo + " = '" + valor + "' WHERE id_empleado = " + to_string(id);
    connector.update(query);
}

void updateProductos(MySQLConnector& connector, const string& tabla) {
    string campo;
    string valor;
    int id;
    cout << "Ingrese el ID del producto que desea modificar: ";
    cin >> id;
    cout << "Seleccione el campo que desea modificar:\n1. Nombre del producto\n2. Tipo de producto\n3. Fecha (ano-dia-mes)\nOpcion: ";
    int opcionCampo;
    cin >> opcionCampo;
    switch (opcionCampo) {

    case 1:
        campo = "nombre";
        break;
    case 2:
        campo = "tipo_producto";
        break;
    case 3:
        campo = "fecha_caducidad";
        break;
    default:
        cout << "Opcion de campo invalida." << endl;
        return;
    }
    cout << "Ingrese el nuevo detalle del objeto " << campo << ": ";
    cin.ignore();
    getline(cin, valor);
    string query = "UPDATE " + tabla + " SET " + campo + " = '" + valor + "' WHERE id_producto = " + to_string(id);
    connector.update(query);
}

void updateVentas(MySQLConnector& connector, const string& tabla) {
    string campo;
    string valor;
    int id;
    cout << "Ingrese el ID de la venta que desea modificar: ";
    cin >> id;
    cout << "Seleccione el campo que desea modificar:\n1. Fecha\n2. Total de venta\n3. NIT del cliente\n4. ID de empleado\nOpcion: ";
    int opcionCampo;
    cin >> opcionCampo;
    switch (opcionCampo) {
    case 1:
        campo = "fecha";
        break;
    case 2:
        campo = "total_venta";
        break;
    case 3:
        campo = "clientes_nit_cliente";
        break;
    case 4:
        campo = "empleados_id_empelado";
        break;
    default:
        cout << "Opcion de campo invalida." << endl;
        return;
    }
    cout << "Ingrese el nuevo detalle del objeto " << campo << ": ";
    cin.ignore();
    getline(cin, valor);
    string query = "UPDATE " + tabla + " SET " + campo + " = '" + valor + "' WHERE id_venta = " + to_string(id);
    connector.update(query);
}

void updateDetallesVentas(MySQLConnector& connector, const string& tabla) {
    string campo;
    string valor;
    int id;
    cout << "Ingrese el ID del detalle de venta que desea modificar: ";
    cin >> id;
    cout << "Seleccione el campo que desea modificar:\n1. Cantidad\n2. Precio\n3. ID del producto\n4. ID de la ventaID_Venta\nOpcion: ";
    int opcionCampo;
    cin >> opcionCampo;
    switch (opcionCampo) {
    case 1:
        campo = "cantidad";
        break;
    case 2:
        campo = "precio";
        break;
    case 3:
        campo = "productos_id_producto";
        break;
    case 4:
        campo = "ventas_id_venta";
        break;
    default:
        cout << "Opcion de campo invalida." << endl;
        return;
    }
    cout << "Ingrese el nuevo detalle del objeto " << campo << ": ";
    cin.ignore();
    getline(cin, valor);
    string query = "UPDATE " + tabla + " SET " + campo + " = '" + valor + "' WHERE id_det_venta = " + to_string(id);
    connector.update(query);
}

void insertarEnProductos(MySQLConnector& connector, const string& tabla) {
    string nombre, descripcion, fecha;
    cout << "Nombre del producto: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Tipo de producto: ";
    getline(cin, descripcion);
    cout << "Fecha de caducidad (ano-dia-mes): ";
    getline(cin, fecha);

    string query = "INSERT INTO " + tabla + " (nombre, tipo_producto, fecha_caducidad) VALUES ('" + nombre + "', '" + descripcion + "', '" + fecha + "')";
    connector.insert(query);

void insertarEnEmpleados(MySQLConnector& connector, const string& tabla) {
    string nombre, apellido, puesto, salario, sexo;
    cout << "Nombre del empleado: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Apellido del empleado: ";
    getline(cin, apellido);
    cout << "Puesto del trabajador: ";
    getline(cin, puesto);
    cout << "Sexo (M/F): ";
    cin >> sexo;
    transform(sexo.begin(), sexo.end(), sexo.begin(), ::toupper);
    while (sexo != "M" && sexo != "F") {
        cout << "Entrada inválida. Por favor ingrese 'M' para Masculino o 'F' para Femenino: ";
        cin >> sexo;
        transform(sexo.begin(), sexo.end(), sexo.begin(), ::toupper);
    }
    string genero = (sexo == "M") ? "Masculino" : "Femenino";
    cout << "Cantidad de salario: Q";
    cin.ignore();
    getline(cin, salario);
    string query = "INSERT INTO " + tabla + " (nombre, apellido, puesto, genero, salario) VALUES ('" + nombre + "','" + apellido + "','" + puesto + "','" + genero + "','" + salario + "')";
    connector.insert(query);
    string mensaje = "Nuevo empleado insertado: " + nombre + " " + apellido;
    escribirHist(mensaje);
}


void insertarEnVentas(MySQLConnector& connector, const string& tabla) {
    int empleado_id;
    string cliente_id, fecha;
    double total;
    cout << "NIT del cliente: ";
    cin.ignore();
    getline(cin, cliente_id);
    cout << "ID del empleado: ";
    cin >> empleado_id;
    cout << "Fecha de Compra (ano-dia-mes): ";
    cin.ignore();
    getline(cin, fecha);
    cout << "Total de la venta: Q";
    cin >> total;

    string query = "INSERT INTO " + tabla + " (fecha, total_venta, clientes_nit_cliente, empleados_id_empleado) VALUES ('" + fecha + "', " + to_string(total) + ",'" + cliente_id + "', " + to_string(empleado_id) +")";
    connector.insert(query);
}

void insertarEnDetalle_venta(MySQLConnector& connector, const string& tabla) {
    int venta_id, producto_id, cantidad;
    double precio;
    cout << "ID de venta: ";
    cin >> venta_id;
    cout << "ID de producto: ";
    cin >> producto_id;
    cout << "Cantidad del producto: ";
    cin >> cantidad;
    cout << "Precio del producto: ";
    cin >> precio;

    string query = "INSERT INTO " + tabla + " (cantidad, precio, productos_id_producto, ventas_id_venta) VALUES (" + to_string(cantidad) + "," + to_string(precio) + ", " + to_string(producto_id) + ", " + to_string(venta_id) + ")";
    connector.insert(query);
}

void escribirHist(const string& mensaje) {
    ofstream archivo("historial.txt", ios::app);
    if (archivo.is_open()) {
        archivo << mensaje << endl;
        archivo.close();
    }
    else {
        cout << "No se pudo abrir el archivo de historial." << endl;
    }
}

void mostrarHist() {
    ifstream archivo("historial.txt");
    if (archivo.is_open()) {
        string linea;
        cout << "Historial de operaciones:\n";
        while (getline(archivo, linea)) {
            cout << linea << endl;
        }
        archivo.close();
    }
    else {
        cout << "No se pudo abrir el archivo de historial." << endl;
    }
}
