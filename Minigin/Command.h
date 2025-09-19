#pragma once

#include <vector>

namespace dae
{
	class ICommand
	{
	public:
		virtual ~ICommand() = default;
		virtual void Execute() = 0;
	};

	//I know this is a different pattern, but I'll just add it for convenience
	class CommandList {

        //Returns false if already possesses the command
        bool Add(const std::shared_ptr<ICommand>& command) {

            //probably better to find like this for command size instead of the extra jump for an unordered map, but not entirely sure
            for (size_t i = 0; i < m_Commands.size(); i++)
            {
                if (m_Commands[i] == command)
                {
                    return false;
                }
            }

            m_Commands.push_back(command);

            return true;
        }

        void Remove(const std::shared_ptr<ICommand>& command) {

            for (size_t i = 0; i < m_Commands.size(); i++)
            {
                if (m_Commands[i] == command)
                {
                    m_Commands.erase(m_Commands.begin() + i);
                    return;
                }
            }
        }

        void Execute() {
            for (const auto& command : m_Commands) {
                command->Execute();
            }
        }

    private:

        std::vector<std::shared_ptr<ICommand>> m_Commands;

	};


}

