#sbs-git:slp/pkgs/l/libjson-glib json-glib 0.0.1 982a2ec62fdaecef7bf0d791b9b7be16d398d70b

Name:       json-glib
Summary:    JSON Parser for GLib
Version: 0.0.1
Release:    1
Group:      System/Libraries
License:    LGPLv2.1
Source0:    %{name}-%{version}.tar.gz
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

%package docs
Summary:    Documentation components for the json-glib package
Group:      Documentation
Requires:   %{name} = %{version}-%{release}

%description docs
Components for the json-glib package (doc)


%prep
%setup -q -n %{name}-%{version}


%build

%reconfigure --disable-static
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/usr/share/license
cp COPYING %{buildroot}/usr/share/license/%{name}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest libsjon-glib.manifest
%defattr(-,root,root,-)
%doc COPYING
/usr/lib/libjson-glib-1.0.so.*

/usr/share/license/%{name}

%files devel
%defattr(-,root,root,-)
/usr/include/json-glib-1.0/json-glib/*.h
/usr/lib/*.so
/usr/lib/pkgconfig/*.pc

%files docs
%defattr(-,root,root,-)
%doc /usr/share/gtk-doc/html/json-glib/*

