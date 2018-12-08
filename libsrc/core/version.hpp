#ifndef NETGEN_CORE_VERSION_HPP
#define NETGEN_CORE_VERSION_HPP

#include <string>
#include <tuple>
#include "ngcore_api.hpp"

namespace ngcore
{
  class Archive;
  class VersionInfo
  {
  private:
    size_t mayor_, minor_, release, patch;
    std::string git_hash;
  public:
    VersionInfo() : mayor_(0), minor_(0), release(0), patch(0), git_hash("") {}
    VersionInfo(std::string vstring)
    {
      minor_ = release = patch = 0;
      git_hash = "";
      if(vstring.substr(0,1) == "v")
        vstring = vstring.substr(1,vstring.size()-1);
      auto dot = vstring.find(".");
      mayor_ = std::stoi(vstring.substr(0,dot));
      if(dot == size_t(-1)) vstring = "";
      else vstring = vstring.substr(dot+1, vstring.size()-dot-1);
      if(vstring.size())
        {
          dot = vstring.find(".");
          minor_ = std::stoi(vstring.substr(0,dot));
          if (dot == size_t(-1)) vstring = "";
          else vstring = vstring.substr(dot+1, vstring.size()-dot-1);
          if(vstring.size())
            {
              dot = vstring.find("-");
              release = std::stoi(vstring.substr(0,dot));
              if(dot == size_t(-1)) vstring = "";
              else vstring = vstring.substr(dot+1,vstring.size()-dot-1);
              if(vstring.size())
                {
                  dot = vstring.find("-");
                  patch = std::stoi(vstring.substr(0,dot));
                  if(dot == size_t(-1)) vstring = "";
                  else vstring = vstring.substr(dot+1, vstring.size()-dot-1);
                  if(vstring.size())
                    git_hash = vstring;
                }
            }
        }
    }
    VersionInfo(const char* cstr) : VersionInfo(std::string(cstr)) { }

    std::string to_string() const
    { std::string vstring = "v" + std::to_string(mayor_);
      if(minor_ || release || patch || git_hash.size())
        {
          vstring += "." + std::to_string(minor_);
          if(release || patch || git_hash.size())
            {
              vstring += "." + std::to_string(release);
              if(patch || git_hash.size())
                {
                  vstring += "-" + std::to_string(patch);
                  if(git_hash.size())
                    vstring += "-" + git_hash;
                }
            }
        }
      return vstring;
    }
    bool operator <(const VersionInfo& other) const
    {
      return std::tie(mayor_, minor_, release, patch) <
        std::tie(other.mayor_, other.minor_, other.release, other.patch);
    }
    bool operator ==(const VersionInfo& other) const
    {
      return mayor_ == other.mayor_ && minor_ == other.minor_ && release == other.release
        && patch == other.patch;
    }
    bool operator >(const VersionInfo& other) const { return other < (*this); }
    bool operator <=(const VersionInfo& other) const { return !((*this) > other); }
    bool operator >=(const VersionInfo& other) const { return !((*this) < other); }

    void DoArchive(Archive& ar);
  };
} // namespace ngcore

#endif // NETGEN_CORE_VERSION_HPP
