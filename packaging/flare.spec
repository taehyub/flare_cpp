Name:       flare
Summary:    Flare Animation Runtime Engine
Version:    0.0.1
Release:    1
Group:      Graphics System/Rendering Engine
License:    MIT
URL:        https://review.tizen.org/platform/core/uifw/flare
Source0:    %{name}-%{version}.tar.gz

BuildRequires:  pkgconfig
BuildRequires:  pkgconfig(thorvg)
BuildRequires:  pkgconfig(elementary)

BuildRequires:  meson
BuildRequires:  ninja
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description
Flare Animation Runtime Engine


%package devel
Summary:    Flare Animation Runtime Engine
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}


%description devel
Flare Animation Runtime Engine (devel)


%prep
%setup -q


%build

export DESTDIR=%{buildroot}

meson setup \
      --prefix /usr \
      --libdir %{_libdir} \
      builddir 2>&1
ninja \
      -C builddir \
      -j %(echo "`/usr/bin/getconf _NPROCESSORS_ONLN`")

%install

export DESTDIR=%{buildroot}

ninja -C builddir install

%files
%defattr(-,root,root,-)
%{_libdir}/libflare.so.*
%manifest packaging/flare.manifest

%files devel
%defattr(-,root,root,-)
%{_includedir}/flare/*.hpp
%{_includedir}/flare/animation/*.hpp
%{_includedir}/flare/animation/interpolators/*.hpp
%{_includedir}/flare/animation/keyframes/*.hpp
%{_includedir}/flare/exceptions/*.hpp
%{_includedir}/flare/paint/*.hpp
%{_includedir}/flare/path/*.hpp
%{_includedir}/flare_math/*.hpp
%{_includedir}/flare_thorvg/*.hpp
%{_libdir}/libflare.so
%{_bindir}/flare
/home/flare/*.flr

%{_libdir}/pkgconfig/flare.pc
