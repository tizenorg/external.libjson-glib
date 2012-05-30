Name:       json-glib
Summary:    JSON Parser for GLib
Version:    0.10.4
Release:    2.8
Group:      System/Libraries
License:    LGPLv2.1
Source0:    json-glib-%{version}.tar.gz
Source1001: packaging/json-glib.manifest 
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(gobject-2.0)
BuildRequires:  pkgconfig(gnutls)


%description
an json-glib api library implementation in C (shared libs)



%package devel
Summary:    Development components for the json-glib package
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
Components for the json-glib package (devel)


%prep
%setup -q 


%build
cp %{SOURCE1001} .

%configure --without-gnome --enable-sqllite=yes
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install




%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig



%docs_package


%files
%manifest json-glib.manifest
%doc COPYING
/usr/lib/libjson-glib-1.0.so.*

%files devel
%manifest json-glib.manifest
/usr/include/json-glib-1.0/json-glib/*.h
/usr/lib/*.so
/usr/lib/pkgconfig/*.pc
